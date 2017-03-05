// Standard include(s)
#include <stdexcept>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/Money.hpp>
#include "SQLExpenseRepository.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
QString SQLExpenseRepository::table_name_ = "budget_expense";

//--------------------------------------------------------------------------------------------------
SQLExpenseRepository::SQLExpenseRepository(QSqlDatabase & db,
                                           std::shared_ptr<ledger::EnvelopeRepository> envelopes)
        : db_(db), envelopes_(envelopes) {
    if (not db_.tables().contains("envelope")) {
        last_error_ = QObject::tr("Envelope table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db_);
    bool success =
            query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ("
                               "id INTEGER PRIMARY KEY, "
                               "amount INTEGER NOT NULL, "
                               "description VARCHAR, "
                               "envelope_id INTEGER NOT NULL, "
                               "beginning_date DATE NOT NULL, "
                               "ending_date DATE, "
                               "day INTEGER DEFAULT 0, "
                               "month INTEGER DEFAULT 0, "
                               "periodicity INTEGER DEFAULT 0, "
                               "scope INTEGER DEFAULT 0, "
                               "week INTEGER DEFAULT 0, "
                               "FOREIGN KEY(envelope_id) REFERENCES envelope(id) ON DELETE RESTRICT);")
                       .arg(table_name_));
    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }
}

//--------------------------------------------------------------------------------------------------
int SQLExpenseRepository::create(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(
            QString("INSERT INTO %1(amount, description, envelope_id, beginning_date, ending_date, "
                    " day, month, periodicity, scope, week) VALUES(:amount, :description, "
                    ":envelope, :start, :stop, :day, :month, :periodicity, :scope, :week);")
                .arg(table_name_));
    query.bindValue(":amount", expense.amount().scaled());
    query.bindValue(":description", expense.description());
    query.bindValue(":envelope", expense.envelope().id());
    query.bindValue(":start", expense.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", expense.endingDate().isValid()
                    ? expense.endingDate().toString("yyyy-MM-dd")
                    : QVariant(QDate()));
    auto rec = expense.recurrence();
    query.bindValue(":day", rec.day());
    query.bindValue(":month", rec.month());
    query.bindValue(":periodicity", rec.periodicity());
    query.bindValue(":scope", static_cast<int>(rec.scope()));
    query.bindValue(":week", rec.week());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
Expense SQLExpenseRepository::expense(int id) {
    QSqlQuery query(db_);
    query.prepare(QString("SELECT * from %1 WHERE id=:id;").arg(table_name_));
    query.bindValue(":id", id);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No budgeted expense found for ID %1").arg(id);
    } else {
        return toExpense(query.record());
    }
    return Expense();
}

//--------------------------------------------------------------------------------------------------
std::vector<Expense> SQLExpenseRepository::expenses(const ledger::Envelope & envelope) {
    std::vector<Expense> expenses;
    QSqlQuery query(db_);
    query.prepare(QString("SELECT * from %1 WHERE envelope_id=:id;").arg(table_name_));
    query.bindValue(":id", envelope.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        expenses.push_back(toExpense(query.record()));
    }
    return expenses;
}

//--------------------------------------------------------------------------------------------------
QString SQLExpenseRepository::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLExpenseRepository::remove(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(QString("DELETE FROM %1 WHERE id=:id;").arg(table_name_));
    query.bindValue(":id", expense.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLExpenseRepository::update(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(
            QString("UPDATE %1 SET amount=:amount, envelope_id=:envelope, "
                    "beginning_date=strftime('%s', :start), ending_date=strftime('%s', :stop), "
                    "day=:day, month=:month, periodicity=:periodicity, scope=:scope, week=:week "
                    "WHERE id=:id;").arg(table_name_));
    query.bindValue(":amount", expense.amount().scaled());
    query.bindValue(":envelope", expense.envelope().id());
    query.bindValue(":start", expense.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", expense.endingDate().toString("yyyy-MM-dd"));
    auto rec = expense.recurrence();
    query.bindValue(":day", rec.day());
    query.bindValue(":month", rec.month());
    query.bindValue(":periodicity", rec.periodicity());
    query.bindValue(":scope", static_cast<int>(rec.scope()));
    query.bindValue(":week", rec.week());
    query.bindValue(":id", expense.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
Expense SQLExpenseRepository::toExpense(const QSqlRecord & record) {
    auto envelope = envelopes_->getEnvelope(record.value("envelope_id").toInt());

    Expense expense(record.value("id").toInt());
    expense.setAmount(ledger::Money(record.value("amount").toInt(), envelope.currency()));
    expense.setDescription(record.value("description").toString());
    expense.setEnvelope(envelope);
    expense.setBeginningDate(QDate::fromString(record.value("beginning_date").toString(), "yyyy-MM-dd"));
    expense.setEndingDate(QDate::fromString(record.value("ending_date").toString(), "yyyy-MM-dd"));

    Recurrence recurrence;
    recurrence.setDay(record.value("day").toInt());
    recurrence.setMonth(record.value("month").toInt());
    recurrence.setPeriodicity(record.value("periodicity").toInt());
    recurrence.setScope(static_cast<Recurrence::ScopeType>(record.value("scope").toInt()));
    recurrence.setWeek(record.value("week").toInt());
    expense.setRecurrence(recurrence);

    return expense;
}

} // budget namespace
} // ub namespace
