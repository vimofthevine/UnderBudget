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

import java.util.Map;

import org.simpleframework.xml.strategy.CycleStrategy;
import org.simpleframework.xml.strategy.TreeStrategy;
import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Value;
import org.simpleframework.xml.stream.NodeMap;

import com.vimofthevine.underbudget.core.budget.estimate.AnalyzableEstimate;
import com.vimofthevine.underbudget.core.budget.estimate.Estimate;

/**
 * A custom cycle strategy that performs the same functionality
 * as org.simpleframework.xml.strategy.CycleStrategy except
 * that the references are applied only to estimate and
 * assignment rule elements, rather than to all elements.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateReferenceStrategy extends CycleStrategy {
	
	/**
	 * This is the strategy that is delegated to by this strategy.
	 */
	private final TreeStrategy strategy;
	
	/**
	 * Constructor for the <code>EstimateReferenceStrategy</code>
	 * object.
	 */
	public EstimateReferenceStrategy()
	{
		super();
		strategy = new TreeStrategy();
	}

	@Override
	public Value read(Type type, NodeMap node, Map map)
	throws Exception
	{
		if (Estimate.class.isAssignableFrom(type.getType()))
			return super.read(type, node, map);
		else if (AnalyzableEstimate.class.isAssignableFrom(type.getType()))
			return super.read(type, node, map);
		else
			return strategy.read(type, node, map);
	}
	
	@Override
	public boolean write(Type type, Object value, NodeMap node, Map map)
	{
		if (value instanceof Estimate)
		{
			boolean result = super.write(type, value, node, map);
			
			// Need to do this after the write so the nodemap is populated
			node.remove("class");
			
			return result;
		}
		else
			return strategy.write(type, value, node, map);
	}
}
