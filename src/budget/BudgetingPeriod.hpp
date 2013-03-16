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

#ifndef BUDGETINGPERIOD_HPP
#define BUDGETINGPERIOD_HPP

// Qt include(s)
#include <QObject>

namespace ub::budget
{
	/**
	 * Date range over which a budget is defined.
	 */
	class BudgetingPeriod : public QObject
	{
	public:
		/**
		 * Budgeting period type enumeration.
		 */
		enum Type
		{
			/** Literal month (e.g., 4/1-4/30) */
			LiteralMonth,
			/** Literal year (e.g., 1/1/2012-12/31/2012) */
			LiteralYear,
			/** Paydate month (first Friday to last Thursday) */
			PaydateMonth,
			/** Custom period */
			Custom,
		};

		/**
		 * Period definition parameters
		 */
		struct Parameters
		{
			/**
			 * Budgeting period type.
			 */
			Type type;

			/**
			 * Parameter #1.
			 *
			 * * Literal Year : 4-digit year
			 * * Literal & Paydate Month : 4-digit year
			 * * Custom : start date
			 */
			QVariant param1;

			/**
			 * Parameter #2.
			 *
			 * * Literal & Paydate Month : month of the year (1-12)
			 * * Custom : end date
			 */
			QVariant param2;

			/**
			 * Parameter #3.
			 */
			QVariant param3;

			/**
			 * Parameter #4.
			 */
			QVariant param4;
		};

		/**
		 * Constructs a new budgeting period.
		 *
		 * @param[in] params period parameters
		 * @param[in] parent parent object
		 */
		BudgetingPeriod(const Parameters& params, QObject* parent = 0);

		/**
		 * Checks if the given date falls within this budgeting period.
		 *
		 * @param[in] date date to be checked
		 * @return whether the given date is within the start and end dates
		 *         of this budgeting period
		 */
		bool contains(const QDate& date) const;

		/**
		 * Creates a command to change this period's definition.
		 *
		 * @param[in] params new period definition parameters
		 * @param[in] parent parent command to be used for grouping
		 */
		QUndoCommand* update(const Parameters& params,
			QUndoCommand* parent = 0) const;

	signals:
		/**
		 * This signal is emitted whenever the definition of
		 * this budgeting period changes.
		 *
		 * @param params current paramters of this period
		 */
		void paramsChanged(const Parameters& params);

	private:
		/** Period definition parameters */
		Parameters myParameters;
		/** Start date */
		QDate startDate;
		/** End date */
		QDate endDate;

		/**
		 * Sets the definition parameters of this period.
		 *
		 * @param[in] newParams new parameters for this period
		 */
		void setParams(const Parameters& newParams);

		/**
		 * Calculates the start and end dates for a literal year period.
		 *
		 * @param[in] params literal year parameters
		 */
		void calculateLiteralYearDates(const Parameters& params);

		/**
		 * Calculates the start and end dates for a literal month period.
		 *
		 * @param[in] params literal month parameters
		 */
		void calculateLiteralMonthDates(const Parameters& params);

		/**
		 * Calculates the start and end dates for a paydate month period.
		 *
		 * @param[in] params paydate month parameters
		 */
		void calculatePaydateMonthDates(const Parameters& params);

		/**
		 * Calculates the start and end dates for a custom month period.
		 *
		 * @param[in] params custom period parameters
		 */
		void calculateCustomDates(const Parameters& params);
	};
}

#endif // BUDGETINGPERIOD_HPP
