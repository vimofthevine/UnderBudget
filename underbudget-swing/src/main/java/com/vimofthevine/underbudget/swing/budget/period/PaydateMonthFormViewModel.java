/*
 * Copyright 2012 Kyle Treubig
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

package com.vimofthevine.underbudget.swing.budget.period;

import javax.swing.ComboBoxModel;
import javax.swing.SpinnerModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.period.Month;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.PaydateMonthPeriod;
import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.swing.widgets.ItemSelectionListener;
import com.vimofthevine.underbudget.swing.widgets.ObservableComboBoxModel;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for views that display
 * an input form for defining a paydate month
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class PaydateMonthFormViewModel implements MonthFormViewModel {
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Budgeting period updater
	 */
	private final BudgetPeriodUpdater updater;
	
	/**
	 * Budgeting period summary document model
	 */
	private final SimpleDocument summaryModel;
	
	/**
	 * Month combo box model
	 */
	private final ObservableComboBoxModel monthModel;
	
	/**
	 * Year spinner model
	 */
	private final SpinnerModel yearModel;
	
	/**
	 * Currently selected month
	 */
	private Month selectedMonth;
	
	/**
	 * Currently selected year;
	 */
	private int selectedYear;
	
	/**
	 * Currently selected month period
	 */
	private MonthlyBudgetingPeriod period;
	
	/**
	 * Constructs a new paydate month form view model.
	 * 
	 * @param bus    event bus
	 * @param budget budgeting being modified
	 */
	PaydateMonthFormViewModel(EventBus bus, Budget budget)
	{
		this.budget = budget;
		updater = new BudgetPeriodUpdater(bus, budget);
		
		if (budget.getDefinition().getPeriod().getType()
			.equals(PeriodType.PAYDATE_MONTH))
		{
			period = (MonthlyBudgetingPeriod) budget.getDefinition().getPeriod();
		}
		else
		{
    		period = new PaydateMonthPeriod();
		}
		
		selectedMonth = period.getMonth();
		selectedYear = period.getYear();
		
		summaryModel = new SimpleDocument();
		summaryModel.setText(period.getDescription());
		
		monthModel = new ObservableComboBoxModel(Month.values());
		monthModel.setSelectedItem(selectedMonth, false);
		monthModel.addItemSelectionListener(new ItemSelectionListener() {
			@Override
            public void itemSelected(Object item)
            {
				if (item instanceof Month)
				{
					selectedMonth = (Month) item;
					update();
				}
            }
		});
		
		yearModel = new SpinnerNumberModel(selectedYear, selectedYear-5,
			selectedYear+5, 1);
		yearModel.addChangeListener(new ChangeListener() {
			@Override
            public void stateChanged(ChangeEvent event)
            {
				selectedYear = (Integer) yearModel.getValue();
				update();
            }
		});
	}
	
	/**
	 * Updates the selected budgeting period definition.
	 */
	@Override
	public void update()
	{
		period = new PaydateMonthPeriod(selectedMonth, selectedYear);
		summaryModel.setText(period.getDescription());
		
		// Avoid unnecessary updating of the budget
		if ( ! period.equals(budget.getDefinition().getPeriod()))
		{
			updater.update(period);
		}
	}
	
	/**
	 * Returns a document representing the
	 * summary of the budgeting period.
	 * 
	 * @return budgeting period summary document
	 */
	@Override
	public Document getSummaryDocument()
	{
		return summaryModel;
	}

	/**
	 * Returns the combo box model for the
	 * paydate month selection.
	 * 
	 * @return month combo box model
	 */
	@Override
	public ComboBoxModel getMonthModel()
	{
		return monthModel;
	}
	
	/**
	 * Returns the spinner model for the
	 * paydate month's year selection.
	 * 
	 * @return year spinner model
	 */
	@Override
	public SpinnerModel getYearModel()
	{
		return yearModel;
	}
	
}
