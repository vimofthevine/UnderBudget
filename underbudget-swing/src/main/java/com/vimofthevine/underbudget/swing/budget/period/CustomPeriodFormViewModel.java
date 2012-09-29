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

import javax.swing.text.Document;

import org.jdesktop.swingx.calendar.DateSelectionModel;
import org.jdesktop.swingx.calendar.DateSelectionModel.SelectionMode;
import org.jdesktop.swingx.calendar.DefaultDateSelectionModel;
import org.jdesktop.swingx.event.DateSelectionEvent;
import org.jdesktop.swingx.event.DateSelectionListener;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.period.CustomPeriod;
import com.vimofthevine.underbudget.core.budget.period.DefaultCustomPeriod;
import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for views that display
 * an input form for defining a custom period.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomPeriodFormViewModel
implements BudgetingPeriodFormViewModel {
	
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
	 * Start date selection model
	 */
	private final DateSelectionModel startDateModel;
	
	/**
	 * End date selection model
	 */
	private final DateSelectionModel endDateModel;
	
	/**
	 * Currently selected start date
	 */
	private SimpleDate startDate;
	
	/**
	 * Currently selected end date
	 */
	private SimpleDate endDate;
	
	/**
	 * Currently selected custom period
	 */
	private CustomPeriod period;
	
	/**
	 * Constructs a new custom period form view model.
	 * 
	 * @param bus    event bus
	 * @param budget budget being modified
	 */
	CustomPeriodFormViewModel(EventBus bus, Budget budget)
	{
		this.budget = budget;
		updater = new BudgetPeriodUpdater(bus, budget);
		
		if (budget.getDefinition().getPeriod()
			.getType().equals(PeriodType.CUSTOM))
		{
			period = (CustomPeriod) budget.getDefinition().getPeriod();
		}
		else
		{
			period = new DefaultCustomPeriod();
		}
		
		startDate = period.getStartDate();
		endDate = period.getEndDate();
		
		summaryModel = new SimpleDocument();
		summaryModel.setText(period.getDescription());
		
		startDateModel = new DefaultDateSelectionModel();
		startDateModel.setSelectionMode(SelectionMode.SINGLE_SELECTION);
		startDateModel.setSelectionInterval(startDate.getTime(), startDate.getTime());
		startDateModel.addDateSelectionListener(new DateSelectionListener() {
			@Override
            public void valueChanged(DateSelectionEvent arg0)
            {
				startDate = new DateTime(startDateModel.getFirstSelectionDate());
				update();
            }
		});
		
		endDateModel = new DefaultDateSelectionModel();
		endDateModel.setSelectionMode(SelectionMode.SINGLE_SELECTION);
		endDateModel.setSelectionInterval(endDate.getTime(), endDate.getTime());
		endDateModel.addDateSelectionListener(new DateSelectionListener() {
			@Override
            public void valueChanged(DateSelectionEvent arg0)
            {
				endDate = new DateTime(endDateModel.getFirstSelectionDate());
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
		period = new DefaultCustomPeriod(startDate, endDate);
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
	public Document getSummaryDocument()
	{
		return summaryModel;
	}
	
	/**
	 * Returns a date selection model representing
	 * the start date of the budgeting period.
	 * 
	 * @return start date selection model
	 */
	public DateSelectionModel getStartDateModel()
	{
		return startDateModel;
	}
	
	/**
	 * Returns a date selection model representing
	 * the end date of the budgeting period.
	 * 
	 * @return end date selection model
	 */
	public DateSelectionModel getEndDateModel()
	{
		return endDateModel;
	}

}
