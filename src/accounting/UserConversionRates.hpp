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

#ifndef USERCONVERSIONRATES_HPP
#define USERCONVERSIONRATES_HPP

// Qt include(s)
#include <QSqlDatabase>
#include <QSqlQuery>

// UnderBudget include(s)
#include "accounting/ConversionRates.hpp"

namespace ub {

/**
 * Source of conversion rates using an SQLite database. All instances of
 * this class will use the same, static internal database instance. This
 * database must have already been opened by a call to `open(QString)` in
 * order to be effective.
 *
 * @ingroup accounting
 */
class UserConversionRates : public ConversionRates
{
public:
	/**
	 * Specifies the SQLite database file name to be used. Note that
	 * changing it will invalidate any queries that exist at the exact
	 * same moment.
	 *
	 * @param[in] filename file name of SQLite database to open
	 */
	static void open(const QString& filename);

	/**
	 * Adds a new conversion rate to the database.
	 *
	 * @param[in] source source currency ISO 4217 code
	 * @param[in] target target currency ISO 4217 code
	 * @param[in] rate   conversion rate
	 */
	void add(const QString& source, const QString& target, double rate);

	/**
	 * Removes the specified conversion rate from the database. The conversion
	 * rate to be removed is specified as a pair of source and target currencies.
	 * If no such pair is found, no action is taken.
	 *
	 * @param[in] source source currency ISO 4217 code
	 * @param[in] target target currency ISO 4217 code
	 */
	void remove(const QString& source, const QString& target);

	/**
	 * Retrieves the conversion rate for the specified source and target
	 * currencies. If no rate is found, a conversion rate of 1 (no change)
	 * is returned.
	 *
	 * @param[in] source source currency ISO 4217 code
	 * @param[in] target target currency ISO 4217 code
	 * @return conversion rate from source currency to target currency, or
	 *         1.0 if no rate is found (no conversion)
	 */
	double get(const QString& source, const QString& target) const;

private:
	/**
	 * Database connection name
	 */
	static const QString connection;

	/**
	 * SQL query to create the table
	 */
	static const QString createTableQuery;

	/**
	 * SQL query to create a unique index between source and target
	 */
	static const QString createIndexQuery;

	/**
	 * SQL query to insert a new conversion rate pair
	 */
	static const QString insertConversionQuery;

	/**
	 * SQL query to delete a conversion rate pair
	 */
	static const QString removeConversionQuery;

	/**
	 * SQL query to retrieve a conversion rate pair
	 */
	static const QString retrieveConversionQuery;
};

}

#endif //USERCONVERSIONRATES_HPP
