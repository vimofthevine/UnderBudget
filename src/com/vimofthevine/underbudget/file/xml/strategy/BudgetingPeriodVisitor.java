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

package com.vimofthevine.underbudget.file.xml.strategy;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Visitor;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.NodeMap;
import org.simpleframework.xml.stream.OutputNode;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.CustomBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.YearlyBudgetingPeriod;

/**
 * Visitor to remove the class attribute from the period element
 * upon serialization and to add the class attribute according
 * to the period type upon deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetingPeriodVisitor implements Visitor {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetingPeriodVisitor.class.getName());

	/**
	 * Adds the class attribute to budgeting period elements,
	 * according to the value of the type attribute.
	 */
	@Override
    public void read(Type arg0, NodeMap<InputNode> arg1) throws Exception
    {
		if (BudgetingPeriod.class.isAssignableFrom(arg0.getType()))
		{
			String type = arg1.get("type").getValue();
			logger.log(Level.FINE, "Reading budgeting period element of type " + type);
			
			if (type.equalsIgnoreCase(CustomBudgetingPeriod.TYPE))
			{
				arg1.put("class", CustomBudgetingPeriod.class.getName());
			}
			else if (type.equalsIgnoreCase(MonthlyBudgetingPeriod.TYPE))
			{
				arg1.put("class", MonthlyBudgetingPeriod.class.getName());
			}
			else if (type.equalsIgnoreCase(YearlyBudgetingPeriod.TYPE))
			{
				arg1.put("class", YearlyBudgetingPeriod.class.getName());
			}
		}
    }

	/**
	 * Removes the class attribute from budgeting period elements.
	 */
	@Override
    public void write(Type arg0, NodeMap<OutputNode> arg1) throws Exception
    {
		if (BudgetingPeriod.class.isAssignableFrom(arg0.getType()))
		{
			logger.log(Level.FINE, "Writing budgeting period node");
			arg1.remove("class");
		}
    }

}
