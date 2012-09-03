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

package com.vimofthevine.underbudget.stubs.budget;

import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.TransactionField;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.PaydateMonthPeriod;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.stubs.assignment.StubAssignmentRule;
import com.vimofthevine.underbudget.stubs.assignment.StubAssignmentRules;
import com.vimofthevine.underbudget.stubs.estimate.StubEstimate;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubBudget implements MutableBudget {
	
	private String name;
	private CashCommodity initial;
	private BudgetingPeriod period;
	private StubEstimate root;
	private StubAssignmentRules rules;
	
	public StubBudget()
	{
		name = "Stub Budget";
		initial = cash("1000.00");
		period = new PaydateMonthPeriod();
		
		root = new StubEstimate("Root", cash("0.00"));
		StubEstimate cat1 = new StubEstimate("Category 1", cash("0.00"));
		StubEstimate cat2 = new StubEstimate("Category 2", cash("0.00"));
		StubEstimate est1 = new StubEstimate("Estimate 1", cash("12.34"));
		StubEstimate est2 = new StubEstimate("Estimate 2", cash("100.00"));
		StubEstimate est3 = new StubEstimate("Estimate 3", cash("75.52"));
		StubEstimate est4 = new StubEstimate("Estimate 4", cash("30.00"));
		StubEstimate est5 = new StubEstimate("Estimate 5", cash("75.00"));
		
		root.add(cat1);
		root.add(cat2);
		cat1.add(est1);
		cat1.add(est2);
		cat2.add(est3);
		cat2.add(est4);
		root.add(est5);
		
		rules = new StubAssignmentRules();
		rules.append(new StubAssignmentRule(est2));
		rules.append(new StubAssignmentRule(est4));
		rules.append(new StubAssignmentRule(est2));
		rules.append(new StubAssignmentRule(est5, TransactionField.DEPOSIT,
			EvaluationOperator.ENDS_WITH, "Groceries"));
		rules.append(new StubAssignmentRule(est1));
		rules.append(new StubAssignmentRule(est5));
		rules.append(new StubAssignmentRule(est3));
	}

	@Override
	public BudgetDefinition getDefinition()
	{
		return new BudgetDefinition() {
			
			@Override
            public String getName()
            {
				return name;
            }

			@Override
            public CashCommodity getInitialBalance()
            {
				return initial;
            }

			@Override
            public BudgetingPeriod getPeriod()
            {
	            return period;
            }
			
		};
	}

	@Override
	public Estimate getRootEstimate()
	{
		return root;
	}

	@Override
	public AssignmentRules getAssignmentRules()
	{
		return rules;
	}

	@Override
    public void setDefinition(BudgetDefinition definition)
    {
		name = definition.getName();
		initial = definition.getInitialBalance();
		period = definition.getPeriod();
    }
	
	private CashCommodity cash(String amount)
	{
		return Commodity.create("USD", amount);
	}

}
