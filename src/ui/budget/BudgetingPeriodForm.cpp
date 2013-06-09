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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/budget/BudgetingPeriodForm.hpp"
#include "ui/widgets/IgnoreUndoRedo.hpp"
#include "ui/widgets/LineEdit.hpp"

namespace ub {

//------------------------------------------------------------------------------
static const QString CALENDAR_YEAR = QObject::tr("A range covering an entire"
	" calendar year (Jan 1st through Dec 31st).");
static const QString CALENDAR_MONTH = QObject::tr("A range covering an entire"
	" calendar month (1st of month through 28th, 30th, or 31st).");
static const QString PAYDATE_MONTH = QObject::tr("A range from the first Friday"
	" of the month through the Thursday before the next month's first Friday.");
static const QString CUSTOM_RANGE = QObject::tr("A range from the specified"
	" start date through the specified end date.");

//------------------------------------------------------------------------------
BudgetingPeriodForm::BudgetingPeriodForm(QSharedPointer<BudgetingPeriod> period,
		QUndoStack* stack, QWidget* parent)
	: QWidget(parent), undoStack(stack), period(period), disableSignals(false)
{
	// Have to use Qt5 style connect because of namespaced type
	connect(period.data(), &BudgetingPeriod::paramsChanged,
		this, &BudgetingPeriodForm::updateWidgets);

	// Setup range display field
	rangeDisplay = new QLineEdit(createRangeText(), this);
	rangeDisplay->setReadOnly(true);

	// Setup type selection field
	typeField = new QComboBox(this);
	typeField->addItem(tr("Calendar Year"), qVariantFromValue(BudgetingPeriod::CalendarYear));
	typeField->addItem(tr("Calendar Month"), qVariantFromValue(BudgetingPeriod::CalendarMonth));
	typeField->addItem(tr("Paydate Month"), qVariantFromValue(BudgetingPeriod::PaydateMonth));
	typeField->addItem(tr("Custom Range"), qVariantFromValue(BudgetingPeriod::CustomDateRange));
	connect(typeField, SIGNAL(currentIndexChanged(int)),
		this, SLOT(updateParams()));

	// Setup description display field
	description = new QTextEdit(this);
	description->setMaximumHeight(50);
	description->setEnabled(false);

	// Setup year selection field
	QDate today = QDate::currentDate();
	yearField = new QSpinBox(this);
	yearField->installEventFilter(new IgnoreUndoRedo(this, this));
	yearField->setMinimum(today.year() - 100);
	yearField->setMaximum(today.year() + 100);
	yearField->setValue(today.year());
	connect(yearField, SIGNAL(valueChanged(QString)),
		this, SLOT(updateParams()));

	// Setup month selection field
	monthField = new QComboBox(this);
	monthField->addItem(tr("January"), 1);
	monthField->addItem(tr("Feburary"), 2);
	monthField->addItem(tr("March"), 3);
	monthField->addItem(tr("April"), 4);
	monthField->addItem(tr("May"), 5);
	monthField->addItem(tr("June"), 6);
	monthField->addItem(tr("July"), 7);
	monthField->addItem(tr("August"), 8);
	monthField->addItem(tr("September"), 9);
	monthField->addItem(tr("October"), 10);
	monthField->addItem(tr("November"), 11);
	monthField->addItem(tr("December"), 12);
	monthField->setCurrentIndex(today.month() - 1);
	connect(monthField, SIGNAL(currentIndexChanged(int)),
		this, SLOT(updateParams()));

	// Setup start date selection field
	startDateField = new QDateEdit(today, this);
	startDateField->installEventFilter(new IgnoreUndoRedo(this, this));
	startDateField->setCalendarPopup(true);
	connect(startDateField, SIGNAL(dateChanged(QDate)),
		this, SLOT(updateParams()));

	// Setup end date selection field
	endDateField = new QDateEdit(today, this);
	endDateField->installEventFilter(new IgnoreUndoRedo(this, this));
	endDateField->setCalendarPopup(true);
	connect(endDateField, SIGNAL(dateChanged(QDate)),
		this, SLOT(updateParams()));

	// Put year and month side-by-side
	QHBoxLayout* monthYear = new QHBoxLayout;
	monthYear->addWidget(monthField);
	monthYear->addWidget(yearField);

	// Put start/end date fields in their own widget
	customRangeForm = new QWidget(this);
	QFormLayout* customRangeLayout = new QFormLayout;
	customRangeLayout->addRow(tr("Start"), startDateField);
	customRangeLayout->addRow(tr("End"), endDateField);
	customRangeForm->setLayout(customRangeLayout);

	// Add to widget
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(rangeDisplay);
	layout->addWidget(typeField);
	layout->addWidget(description);
	layout->addLayout(monthYear);
	layout->addWidget(customRangeForm);
	setLayout(layout);

	updateWidgets(period->parameters());
}

//------------------------------------------------------------------------------
QString BudgetingPeriodForm::createRangeText() const
{
	return QString("%1 to %2").arg(period->startDate().toString())
		.arg(period->endDate().toString());
}

//------------------------------------------------------------------------------
const QString& BudgetingPeriodForm::typeDescription(const BudgetingPeriod::Type& type)
{
	switch (type)
	{
		case BudgetingPeriod::CalendarYear:
			return CALENDAR_YEAR;
		case BudgetingPeriod::CalendarMonth:
			return CALENDAR_MONTH;
		case BudgetingPeriod::PaydateMonth:
			return PAYDATE_MONTH;
		case BudgetingPeriod::CustomDateRange:
			return CUSTOM_RANGE;
	}

	static QString defaultDescription;
	return defaultDescription;
}

//------------------------------------------------------------------------------
void BudgetingPeriodForm::displayWidgets(const BudgetingPeriod::Type& type)
{
	switch (type)
	{
		case BudgetingPeriod::CalendarYear:
			yearField->show();
			monthField->hide();
			customRangeForm->hide();
			break;
		case BudgetingPeriod::CalendarMonth:
			yearField->show();
			monthField->show();
			customRangeForm->hide();
			break;
		case BudgetingPeriod::PaydateMonth:
			yearField->show();
			monthField->show();
			customRangeForm->hide();
			break;
		case BudgetingPeriod::CustomDateRange:
			yearField->hide();
			monthField->hide();
			customRangeForm->show();
			break;
	}
}

//------------------------------------------------------------------------------
void BudgetingPeriodForm::updateWidgets(const BudgetingPeriod::Parameters& params)
{
	// Make sure we don't try to re-set parameters when updating the widgets
	disableSignals = true;

	rangeDisplay->setText(createRangeText());
	typeField->setCurrentIndex(typeField->findData(qVariantFromValue(params.type)));
	description->setText(typeDescription(params.type));
	displayWidgets(params.type);

	switch (params.type)
	{
	case BudgetingPeriod::CalendarYear:
		yearField->setValue(params.param1.toInt());
		break;
	case BudgetingPeriod::CalendarMonth:
		yearField->setValue(params.param1.toInt());
		monthField->setCurrentIndex(params.param2.toInt() - 1);
		break;
	case BudgetingPeriod::PaydateMonth:
		yearField->setValue(params.param1.toInt());
		monthField->setCurrentIndex(params.param2.toInt() - 1);
		break;
	case BudgetingPeriod::CustomDateRange:
		startDateField->setDate(params.param1.toDate());
		endDateField->setDate(params.param2.toDate());
		break;
	}

	// Make sure user-input will update the period parameters
	disableSignals = false;
}

//------------------------------------------------------------------------------
void BudgetingPeriodForm::updateParams()
{
	if ( ! disableSignals)
	{
		BudgetingPeriod::Type type = typeField->itemData(typeField->currentIndex())
			.value<BudgetingPeriod::Type>();

		description->setText(typeDescription(type));
		displayWidgets(type);

		BudgetingPeriod::Parameters params;
		params.type = type;

		switch (type)
		{
		case BudgetingPeriod::CalendarYear:
			params.param1 = yearField->value();
			break;
		case BudgetingPeriod::CalendarMonth:
			params.param1 = yearField->value();
			params.param2 = monthField->currentIndex() + 1;
			break;
		case BudgetingPeriod::PaydateMonth:
			params.param1 = yearField->value();
			params.param2 = monthField->currentIndex() + 1;
			break;
		case BudgetingPeriod::CustomDateRange:
			params.param1 = startDateField->date();
			params.param2 = endDateField->date();
			break;
		}

		undoStack->push(period->update(params));
	}
}

}

