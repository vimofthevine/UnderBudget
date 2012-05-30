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

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.USDCurrency;

/**
 * Visitor to remove the class attribute from the currency element
 * upon serialization and to add the class attribute according
 * to the currency type upon deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencyVisitor implements Visitor {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CurrencyVisitor.class.getName());
	
	/**
	 * Adds the class attribute to currency elements,
	 * according to the value of the currency type attribute.
	 */
	@Override
    public void read(Type arg0, NodeMap<InputNode> arg1) throws Exception
    {
		if (Currency.class.isAssignableFrom(arg0.getType()))
		{
			String type = arg1.get("currency") == null
				? USDCurrency.TYPE : arg1.get("currency").getValue();
			logger.log(Level.FINE, "Reading currency element of type " + type);
			
			if (type.equalsIgnoreCase(USDCurrency.TYPE))
			{
				arg1.put("class", USDCurrency.class.getName());
			}
		}
    }

	/**
	 * Removes the class attribute from currency elements
	 */
	@Override
    public void write(Type arg0, NodeMap<OutputNode> arg1) throws Exception
    {
		if (Currency.class.isAssignableFrom(arg0.getType()))
		{
			logger.log(Level.FINE, "Writing currency node");
			arg1.remove("class");
		}
    }

}
