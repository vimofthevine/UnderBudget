/*
 * Copyright 2016 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Standard include(s)
#include <iostream>
#include <map>
#include <stack>
#include <stdexcept>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include "SQLEnvelopeRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
QString SQLEnvelopeRepository::table_name_ = "envelope";

//--------------------------------------------------------------------------------------------------
SQLEnvelopeRepository::SQLEnvelopeRepository(QSqlDatabase & db) : db_(db) {
    if (not db.tables().contains("currency")) {
        last_error_ = QObject::tr("Currency table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS " + table_name_ +
                              "("
                              "id INTEGER PRIMARY KEY, "
                              "name VARCHAR NOT NULL, "
                              "currency_id INTEGER DEFAULT 1, "
                              "lft INTEGER NOT NULL, "
                              "rgt INTEGER NOT NULL, "
                              "archived BOOLEAN DEFAULT 0, "
                              "FOREIGN KEY(currency_id) REFERENCES currency(id) "
                              "ON DELETE SET DEFAULT);");

    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }

    query.exec("SELECT id FROM " + table_name_ + " WHERE id=1;");
    if (not query.first() and not query.isValid()) {
        query.prepare("INSERT INTO " + table_name_ +
                      "(id, name, lft, rgt) VALUES(1, 'root', 1, 2);");
        if (not query.exec()) {
            last_error_ = query.lastError().text();
            throw std::runtime_error(last_error_.toStdString());
        }
    }

    cache();
}

//--------------------------------------------------------------------------------------------------
void SQLEnvelopeRepository::cache() {
    envelopes_.clear();

    QSqlQuery query(db_);
    query.exec("SELECT envelope.id,envelope.name,envelope.currency_id,currency.code,envelope.lft,"
               "envelope.rgt FROM " +
               table_name_ + " JOIN currency on envelope.currency_id=currency.id ORDER BY lft;");

    QSqlRecord record;

    std::stack<NestedSetEnvelope> parents;
    while (query.next()) {
        record = query.record();

        NestedSetEnvelope envelope(record.value("id").toInt());
        envelope.setName(record.value("name").toString());
        envelope.setCurrency(
            Currency(record.value("currency_id").toInt(), record.value("code").toString()));
        envelope.lft = record.value("lft").toInt();
        envelope.rgt = record.value("rgt").toInt();

        // If not the root
        if (envelope.id() > 1) {
            // If this envelope is not a child of the current parent, move back up the tree
            while (envelope.lft > parents.top().rgt) {
                parents.pop();
            }

            envelope.setParent(parents.top().id());
            auto children = envelopes_[envelope.parent()].children();
            children.push_back(envelope.id());
            envelopes_[envelope.parent()].setChildren(children);
        }
        envelopes_[envelope.id()] = envelope;

        // Push this envelope onto the stack as the next (potential) parent
        parents.push(envelope);
    }
}

//--------------------------------------------------------------------------------------------------
int SQLEnvelopeRepository::create(const Envelope & envelope, const Envelope & parent) {
    auto iter = envelopes_.find(parent.id());
    if (iter == envelopes_.end()) {
        last_error_ = QObject::tr("No parent envelope found with ID %1").arg(parent.id());
        return -1;
    }
    int rgt = iter->second.rgt;

    db_.transaction();
    QSqlQuery query(db_);

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+2 WHERE lft>=:lft;");
    query.bindValue(":lft", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+2 WHERE rgt>=:rgt;");
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("INSERT INTO " + table_name_ + "(name, currency_id, lft, rgt) "
                                                 "VALUES(:name, :currency, :lft, :rgt);");
    query.bindValue(":name", envelope.name());
    query.bindValue(":currency", envelope.currency().id());
    query.bindValue(":lft", rgt);
    query.bindValue(":rgt", rgt + 1);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    db_.commit();
    cache();
    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
Envelope SQLEnvelopeRepository::getEnvelope(int id) {
    auto iter = envelopes_.find(id);
    if (iter != envelopes_.end()) {
        return iter->second;
    } else {
        last_error_ = QObject::tr("No envelope exists with ID %1").arg(id);
        return Envelope(-1);
    }
}

//--------------------------------------------------------------------------------------------------
std::vector<Envelope> SQLEnvelopeRepository::getLeafEnvelopes() {
    std::vector<Envelope> envelopes;

    QSqlQuery query(db_);
    query.prepare("SELECT id FROM " + table_name_ + " WHERE rgt=lft+1 ORDER BY name;");
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        int id = query.record().value("id").toInt();
        // Don't ever include the root envelope
        if (id == 1) {
            continue;
        }
        auto iter = envelopes_.find(id);
        if (iter != envelopes_.end()) {
            envelopes.push_back(iter->second);
        }
    }

    return envelopes;
}

//--------------------------------------------------------------------------------------------------
Envelope SQLEnvelopeRepository::getRoot() {
    return getEnvelope(1);
}

//--------------------------------------------------------------------------------------------------
QString SQLEnvelopeRepository::lastError() {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLEnvelopeRepository::move(const Envelope & envelope, const Envelope & parent) {
    auto iter = envelopes_.find(envelope.id());
    if (iter == envelopes_.end()) {
        last_error_ = QObject::tr("No envelope found with ID %1").arg(envelope.id());
        return false;
    }
    auto acct = iter->second;

    iter = envelopes_.find(envelope.parent());
    if (iter == envelopes_.end()) {
        last_error_ = QObject::tr("No old parent envelope found with ID %1").arg(envelope.parent());
        return false;
    }
    auto oparent = iter->second;

    iter = envelopes_.find(parent.id());
    if (iter == envelopes_.end()) {
        last_error_ = QObject::tr("No new parent envelope found with ID %1").arg(parent.id());
        return false;
    }
    auto nparent = iter->second;

    // Using algorithm from https://rogerkeays.com/how-to-move-a-node-in-nested-sets-with-sql
    int newlft = nparent.rgt;
    int width = acct.rgt - acct.lft + 1;
    int distance = newlft - acct.lft;
    int tmppos = acct.lft;

    // Account for new space when moving backwards
    if (distance < 0) {
        distance -= width;
        tmppos += width;
    }

    db_.transaction();
    QSqlQuery query(db_);

    // First make space for the subtree

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+:width WHERE lft>=:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", newlft);
    if (not query.exec()) {
        last_error_ = "Unable to update lft to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+:width WHERE rgt>=:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", newlft);
    if (not query.exec()) {
        last_error_ = "Unable to update rgt to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Move subtree into that space

    query.prepare("UPDATE " + table_name_ +
                  " SET lft=lft+:distance, "
                  "rgt=rgt+:distance WHERE lft>=:pos AND rgt<:pos+:width");
    query.bindValue(":distance", distance);
    query.bindValue(":width", width);
    query.bindValue(":pos", tmppos);
    if (not query.exec()) {
        last_error_ = "Unable to move subtree: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", acct.rgt);
    if (not query.exec()) {
        last_error_ = "Unable to update lft to remove space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", acct.rgt);
    if (not query.exec()) {
        last_error_ = "Unable to update rgt to remove space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();
    cache();
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLEnvelopeRepository::remove(const Envelope & envelope) {
    auto iter = envelopes_.find(envelope.id());
    if (iter == envelopes_.end()) {
        last_error_ = QObject::tr("No envelope found with ID %1").arg(envelope.id());
        return false;
    }
    int lft = iter->second.lft;
    int rgt = iter->second.rgt;
    int width = rgt - lft + 1;

    db_.transaction();
    QSqlQuery query(db_);

    // Delete the subtree
    query.prepare("DELETE FROM " + table_name_ + " WHERE lft>=:lft AND rgt<=:rgt");
    query.bindValue(":lft", lft);
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();
    cache();
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLEnvelopeRepository::update(const Envelope & envelope) {
    QSqlQuery query(db_);
    query.prepare("UPDATE " + table_name_ + " SET name=:name, "
                                            "currency_id=:currency WHERE id=:id;");
    query.bindValue(":name", envelope.name());
    query.bindValue(":currency", envelope.currency().id());
    query.bindValue(":id", envelope.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    cache();
    return true;
}

} // ledger namespace
} // ub namespace
