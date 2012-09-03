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

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Version;
import org.simpleframework.xml.convert.Convert;

import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.xml.assignment.XmlAssignmentRules;
import com.vimofthevine.underbudget.xml.budget.period.XmlBudgetingPeriod;
import com.vimofthevine.underbudget.xml.budget.period.XmlBudgetingPeriodFactory;
import com.vimofthevine.underbudget.xml.currency.CommodityConverter;
import com.vimofthevine.underbudget.xml.estimate.XmlEstimate;

/**
 * A <code>Budget</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="budget")
public class XmlBudget implements MutableBudget {
	
	/**
	 * Budget definition version
	 */
	@Version(revision=4.0)
	public final static double version = 4;
	
	/**
	 * Name of this budget
	 */
	@Element
	private String name;
	
	/**
	 * Budgeting period covered by this budget
	 */
	@Element
	private XmlBudgetingPeriod period;
	
	/**
	 * Initial balance
	 */
	@Element(name="initial-balance")
	@Convert(CommodityConverter.class)
	private CashCommodity initialBalance;
	
	/**
	 * Root estimate
	 */
	@Element
	private XmlEstimate estimate;
	
	/**
	 * Assignment rules
	 */
	@Element
	private XmlAssignmentRules rules;
	
	/**
	 * Constructs a new XML budget
	 */
	public XmlBudget()
	{
		rules = new XmlAssignmentRules();
	}
	
	/**
	 * Constructs a new XML budget
	 * based on the given budget.
	 * 
	 * @param budget original budget
	 */
	public XmlBudget(Budget budget)
	{
		BudgetDefinition definition = budget.getDefinition();
		name = definition.getName();
		initialBalance = definition.getInitialBalance();
		period = XmlBudgetingPeriodFactory.create(definition.getPeriod());
		
		estimate = new XmlEstimate(budget.getRootEstimate());
		rules = new XmlAssignmentRules(budget.getAssignmentRules(), estimate);
	}
	
	@Override
	public BudgetDefinition getDefinition()
	{
		return new BudgetDefinition() {
			@Override
            public String getName() { return name; }

			@Override
            public CashCommodity getInitialBalance() { return initialBalance; }

			@Override
            public BudgetingPeriod getPeriod() { return period; }
		};
	}

	@Override
	public Estimate getRootEstimate()
	{
		return estimate;
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
		initialBalance = definition.getInitialBalance();
		period = XmlBudgetingPeriodFactory.create(definition.getPeriod());
    }

}
