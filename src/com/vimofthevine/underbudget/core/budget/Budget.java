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

package com.vimofthevine.underbudget.core.budget;

import java.util.ArrayList;
import java.util.List;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Version;
import org.simpleframework.xml.convert.Convert;

import com.vimofthevine.underbudget.core.budget.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.budget.assignment.UserAssignmentRule;
import com.vimofthevine.underbudget.core.budget.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.core.budget.estimate.Estimate;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * A <code>Budget</code> is the user-defined set of estimated
 * incomes and expenses for a given period of time.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root
public class Budget {
	
	/**
	 * Budget definition version
	 */
	@Version(revision=4.0)
	public final static double version = 4;
	
	/**
	 * Name of this budget
	 */
	@Element(required=false)
	@Convert(TemplateNameConverter.class)
	private String name;
	
	/**
	 * Budgeting period covered by this budget
	 */
	@Element(required=false)
	@Convert(TemplatePeriodConverter.class)
	private BudgetingPeriod period;
	
	/**
	 * Initial financial balance prior to the
	 * start of this budget's budgeting period
	 */
	@Element(name="initial-balance",required=false)
	@Convert(TemplateBalanceConverter.class)
	private Currency initial;
	
	/**
	 * Root estimate
	 */
	@Element
	DefaultEstimate estimate;
	
	/**
	 * Assignment rule list
	 */
	@ElementList(entry="rule",type=UserAssignmentRule.class)
	ArrayList<AssignmentRule> rules;
	
	public Budget(String name, BudgetingPeriod period, Currency initial,
		DefaultEstimate estimate, ArrayList<AssignmentRule> rules)
	{
		this.name = name;
		this.period = period;
		this.initial = initial;
		this.estimate = estimate;
		this.rules = rules;
	}
	
	/**
	 * Returns the name of this budget
	 * 
	 * @return budget name
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 * Returns the budgeting period of this budget
	 * 
	 * @return budgeting period
	 */
	public BudgetingPeriod getBudgetingPeriod()
	{
		return period;
	}
	
	/**
	 * Returns the initial balance of this budget
	 * 
	 * @return initial balance
	 */
	public Currency getInitialBalance()
	{
		return initial.clone();
	}
	
	/**
	 * Returns the root estimate for this budget
	 * 
	 * @return root estimate node defining
	 *          the estimate tree
	 */
	public Estimate getRootEstimate()
	{
		return estimate;
	}
	
	/**
	 * Returns the list of user assignment rules
	 * 
	 * @return list of assignment rules
	 */
	public List<AssignmentRule> getAssignmentRules()
	{
		return rules;
	}
	
	@Override
	public int hashCode()
	{
		int result = 16;
		result = result * 31 + name.hashCode();
		result = result * 31 + period.hashCode();
		result = result * 31 + initial.hashCode();
		result = result * 31 + estimate.hashCode();
		result = result * 31 + rules.hashCode();
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		
		if ( ! (obj instanceof Budget))
			return false;
		
		Budget that = (Budget) obj;
		
		return this.name.equals(that.name)
			&& this.period.equals(that.period)
			&& this.initial.equals(that.initial)
			&& this.estimate.equals(that.estimate)
			&& this.rules.equals(that.rules);
	}
	
	@Override
	public String toString()
	{
		return "[" + name + ": period=" + period + ", initial balance=" + initial
			+ ", root estimate=" + estimate + "rules=" + rules.size() + "]";
	}

}
