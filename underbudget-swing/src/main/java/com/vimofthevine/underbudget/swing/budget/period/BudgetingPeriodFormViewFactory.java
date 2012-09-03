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

import java.awt.Component;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;

/**
 * Factory for building the budgeting period definition
 * section of the edit-budget form view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class BudgetingPeriodFormViewFactory {

	/**
	 * Builds a budgeting period definition form view
	 * instance.
	 * 
	 * @param bus    event bus
	 * @param budget budget being modified
	 * @return budgeting period form view component
	 */
	public static Component build(EventBus bus, Budget budget)
	{
		JPanel periodComponent = new JPanel();
		BudgetingPeriodFormView forms =
			new BudgetingPeriodFormView(periodComponent);
		
		LiteralMonthFormViewModel literalMonthModel =
			new LiteralMonthFormViewModel(bus, budget);
		JPanel literalMonthComponent = new JPanel();
		new MonthFormView(literalMonthComponent, literalMonthModel);
		forms.addForm(PeriodType.LITERAL_MONTH, literalMonthComponent);
		
		PaydateMonthFormViewModel paydateMonthModel =
			new PaydateMonthFormViewModel(bus, budget);
		JPanel paydateMonthComponent = new JPanel();
		new MonthFormView(paydateMonthComponent, paydateMonthModel);
		forms.addForm(PeriodType.PAYDATE_MONTH, paydateMonthComponent);
		
		CustomPeriodFormViewModel customModel =
			new CustomPeriodFormViewModel(bus, budget);
		JPanel customComponent = new JPanel();
		new CustomPeriodFormView(customComponent, customModel);
		forms.addForm(PeriodType.CUSTOM, customComponent);
		
		return periodComponent;
	}
	
}
