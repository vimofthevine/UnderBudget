/*
 * Copyright 2013 Kyle Treubig
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

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include "accounting/UserConversionRates.hpp"

namespace ub {

//------------------------------------------------------------------------------
const QString UserConversionRates::connection = "UserConversionRates";
const QString UserConversionRates::createTableQuery =
	"CREATE TABLE IF NOT EXISTS rates "
	"(id INTEGER PRIMARY KEY, source TEXT, target TEXT, rate REAL);";
const QString UserConversionRates::createIndexQuery =
	"CREATE UNIQUE INDEX IF NOT EXISTS source_target ON rates "
	"(source, target);";
const QString UserConversionRates::insertConversionQuery =
	"INSERT OR REPLACE INTO rates (source, target, rate) "
	"VALUES(:source, :target, :rate);";
const QString UserConversionRates::removeConversionQuery =
	"DELETE FROM rates WHERE source=:source AND target=:target;";
const QString UserConversionRates::retrieveConversionQuery =
	"SELECT rate FROM rates WHERE source=:source AND target=:target;";

//------------------------------------------------------------------------------
void UserConversionRates::open(const QString& filename)
{
	// make sure connection is registered
	QSqlDatabase db = QSqlDatabase::database(connection, false);
	if ( ! db.isValid())
	{
		db = QSqlDatabase::addDatabase("QSQLITE", connection);
	}

	// make sure previous database is closed
	if (db.isOpen())
	{
		// if any other database instances exist right at this moment,
		// they also will be closed and their queries invalidated
		db.close();
	}

	// open database file
	db.setDatabaseName(filename);
	if ( ! db.open())
	{
		qWarning() << "Unable to open conversion rates file, " << filename;
	}
	else
	{
		QSqlQuery createQuery(createTableQuery, db);
		createQuery.exec();

		if (createQuery.lastError().isValid())
		{
			qWarning() << "Error creating conversion rates table: "
				<< createQuery.lastError();
		}
		else
		{
			QSqlQuery indexQuery(createIndexQuery, db);
			indexQuery.exec();

			if (indexQuery.lastError().isValid())
			{
				qWarning() << "Error creating unique index: "
					<< indexQuery.lastError();
			}
		}
	}
}

//------------------------------------------------------------------------------
void UserConversionRates::add(const QString& source, const QString& target,
		double rate)
{
	QSqlQuery insertQuery(QSqlDatabase::database(connection));
	insertQuery.prepare(insertConversionQuery);
	insertQuery.bindValue(":source", source);
	insertQuery.bindValue(":target", target);
	insertQuery.bindValue(":rate", rate);
	insertQuery.exec();

	if (insertQuery.lastError().isValid())
	{
		qWarning() << insertQuery.lastError();
	}
}

//------------------------------------------------------------------------------
void UserConversionRates::remove(const QString& source, const QString& target)
{
	QSqlQuery removeQuery(QSqlDatabase::database(connection));
	removeQuery.prepare(removeConversionQuery);
	removeQuery.bindValue(":source", source);
	removeQuery.bindValue(":target", target);
	removeQuery.exec();

	if (removeQuery.lastError().isValid())
	{
		qWarning() << removeQuery.lastError();
	}
}

//------------------------------------------------------------------------------
double UserConversionRates::get(const QString& source, const QString& target) const
{
	// If no conversion necessary
	if (source == target)
	{
		return 1.0;
	}

	QSqlQuery retrieveQuery(QSqlDatabase::database(connection));
	retrieveQuery.prepare(retrieveConversionQuery);
	retrieveQuery.bindValue(":source", source);
	retrieveQuery.bindValue(":target", target);
	retrieveQuery.exec();

	if (retrieveQuery.next())
	{
		return retrieveQuery.value(0).toDouble();
	}
	else
	{
		return 1.0;
	}
}

}

