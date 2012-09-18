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

package com.vimofthevine.underbudget.xml.budget;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;

/**
 * Template XML budget with minor differences from the
 * original budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlTemplateBudget extends XmlBudget {
	
	/**
	 * Constructs a new template budget from
	 * the given budget.
	 * 
	 * @param budget original budget
	 */
	public XmlTemplateBudget(Budget budget)
	{
		super(budget);
		
		// Replace budget params with template params
		final BudgetDefinition definition = getDefinition();
		setDefinition(new BudgetDefinition() {
			@Override
            public String getName() { return "New Budget"; }
			@Override
            public CashCommodity getInitialBalance()
            {
				return Commodity.zero(
					definition.getInitialBalance().getCurrency());
            }
			@Override
            public BudgetingPeriod getPeriod() { return definition.getPeriod(); }
		});
		
		// Mark all estimates as incomplete
		setEstimateIncomplete((MutableEstimate) getRootEstimate());
	}
	
	/**
	 * Recursively sets an estimate and all its children
	 * as incomplete.
	 * 
	 * @param estimate estimate to be marked as incomplete
	 */
	private void setEstimateIncomplete(MutableEstimate estimate)
	{
		final EstimateDefinition definition = estimate.getDefinition();
		estimate.setDefinition(new EstimateDefinition() {
			@Override
            public String getName() { return definition.getName(); }
			@Override
            public String getDescription() { return definition.getDescription(); }
			@Override
            public CashCommodity getAmount() { return definition.getAmount(); }
			@Override
            public SimpleDate getDueDate() { return definition.getDueDate(); }
			@Override
            public EstimateType getType() { return definition.getType(); }
			@Override
            public boolean isComplete() { return false; }
		});
		
		for (int i=0; i<estimate.getChildCount(); i++)
		{
			setEstimateIncomplete((MutableEstimate) estimate.getChildAt(i));
		}
	}

}
