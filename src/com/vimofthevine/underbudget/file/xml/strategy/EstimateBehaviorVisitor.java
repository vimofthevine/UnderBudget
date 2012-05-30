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

import com.vimofthevine.underbudget.core.budget.estimate.behavior.EstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.GenericEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.OwedEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.SpecificEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.UnbudgetedEstimateBehavior;

/**
 * Visitor to remove the class attribute from the behavior element
 * upon serialization and to add the class attribute according
 * to the behavior type upon deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateBehaviorVisitor implements Visitor {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(EstimateBehaviorVisitor.class.getName());

	/**
	 * Adds the class attribute to estimate behavior elements,
	 * according to the value of the type attribute.
	 */
	@Override
    public void read(Type type, NodeMap<InputNode> node) throws Exception
    {
		if (EstimateBehavior.class.isAssignableFrom(type.getType()))
		{
			String behaviorType = node.get("type").getValue();
			logger.log(Level.FINE, "Reading estimate behavior element of type " + behaviorType);
			
			if (behaviorType.equalsIgnoreCase(GenericEstimateBehavior.TYPE))
			{
				node.put("class", GenericEstimateBehavior.class.getName());
			}
			else if (behaviorType.equalsIgnoreCase(OwedEstimateBehavior.TYPE))
			{
				node.put("class", OwedEstimateBehavior.class.getName());
			}
			else if (behaviorType.equalsIgnoreCase(SpecificEstimateBehavior.TYPE))
			{
				node.put("class", SpecificEstimateBehavior.class.getName());
			}
			else if (behaviorType.equalsIgnoreCase(UnbudgetedEstimateBehavior.TYPE))
			{
				node.put("class", UnbudgetedEstimateBehavior.class.getName());
			}
		}
    }

	/**
	 * Removes the class attribute from estimate behavior elements.
	 */
	@Override
    public void write(Type type, NodeMap<OutputNode> node) throws Exception
    {
		if (EstimateBehavior.class.isAssignableFrom(type.getType()))
		{
			logger.log(Level.FINE, "Writing estimate behavior node");
			node.remove("class");
		}
    }

}
