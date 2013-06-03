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
#include <QDate>
#include <QObject>
#include <QVariant>

// Forward declaration(s)
class QUndoCommand;

namespace ub
{

/**
 * Date range over which a budget is applied.
 */
class BudgetingPeriod : public QObject
{
	Q_OBJECT

public:
	/**
	 * Budgeting period type enumeration.
	 */
	enum Type
	{
		/** Calendar month (e.g., 4/1-4/30) */
		CalendarMonth,
		/** Calendar year (e.g., 1/1/2012-12/31/2012) */
		CalendarYear,
		/** Paydate month (first Friday of month to last Thursday of month) */
		PaydateMonth,
		/** Custom period */
		CustomDateRange,
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
		 * * Calendar Year : 4-digit year
		 * * Calendar & Paydate Month : 4-digit year
		 * * Custom : start date
		 */
		QVariant param1;

		/**
		 * Parameter #2.
		 *
		 * * Calendar & Paydate Month : month of the year (1-12)
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
	 * Constructs a new budgeting period for the current calendar month.
	 *
	 * @param[in] parent parent object
	 */
	BudgetingPeriod(QObject* parent = 0);

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
		QUndoCommand* parent = 0);

	/**
	 * Returns the current parameters defining this budgeting period.
	 *
	 * @return budgeting period parameters
	 */
	Parameters parameters() const;

	/**
	 * Returns the calculated start date of this budgeting period.
	 *
	 * @return start date of this budgeting period
	 */
	QDate startDate() const;

	/**
	 * Returns the calculated end date of this budgeting period.
	 *
	 * @return end date of this budgeting period
	 */
	QDate endDate() const;

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
	Parameters periodParameters;
	/** Start date */
	QDate periodStartDate;
	/** End date */
	QDate periodEndDate;

	/**
	 * Sets the definition parameters of this period.
	 *
	 * @param[in] newParams new parameters for this period
	 */
	void setParams(const Parameters& newParams);

	/**
	 * Calculates the start and end dates for a calendar year period.
	 */
	void calculateCalendarYearDates();

	/**
	 * Calculates the start and end dates for a calendar month period.
	 */
	void calculateCalendarMonthDates();

	/**
	 * Calculates the start and end dates for a paydate month period.
	 */
	void calculatePaydateMonthDates();

	/**
	 * Calculates the start and end dates for a custom month period.
	 */
	void calculateCustomDates();

	// Allow undoable commands direct field access
	friend class ChangePeriodParamsCommand;
};

}

#endif // BUDGETINGPERIOD_HPP
