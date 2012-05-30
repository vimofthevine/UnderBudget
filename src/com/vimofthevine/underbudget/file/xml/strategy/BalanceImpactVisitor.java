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

import com.vimofthevine.underbudget.core.budget.estimate.impact.BalanceImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.ExpenseImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.IncomeImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.NoImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.TransferImpact;

/**
 * Visitor to remove the class attribute from the balance impact
 * upon serialization and to add the class attribute according
 * to the impact type upon deserialization. If the balance impact
 * is none (NoImpact), then the impact element is removed completely
 * upon serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BalanceImpactVisitor implements Visitor {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BalanceImpactVisitor.class.getName());

	@Override
    public void read(Type type, NodeMap<InputNode> node) throws Exception
    {
		if (BalanceImpact.class.isAssignableFrom(type.getType()))
		{
    		String impactType = node.getNode().getValue();
    		logger.log(Level.FINE, "Reading balance impact element of type " + impactType);
    		
    		if (impactType.equalsIgnoreCase(ExpenseImpact.TYPE))
    		{
    			node.put("class", ExpenseImpact.class.getName());
    		}
    		else if (impactType.equalsIgnoreCase(IncomeImpact.TYPE))
    		{
    			node.put("class", IncomeImpact.class.getName());
    		}
    		else if (impactType.equalsIgnoreCase(NoImpact.TYPE))
    		{
    			node.put("class", NoImpact.class.getName());
    		}
    		else if (impactType.equalsIgnoreCase(TransferImpact.TYPE))
    		{
    			node.put("class", TransferImpact.class.getName());
    		}
		}
    }

	/**
	 * Removes the class attribute from balance impact elements.
	 */
	@Override
    public void write(Type type, NodeMap<OutputNode> node) throws Exception
    {
		if (BalanceImpact.class.isAssignableFrom(type.getType()))
		{
			logger.log(Level.FINE, "Writing balance impact node");
			String impactClass = node.get("class").getValue();
			node.remove("class");
			
			if (NoImpact.class.equals(impactClass))
			{
				node.getNode().remove();
			}
		}
    }

}
