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

import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Visitor;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.NodeMap;
import org.simpleframework.xml.stream.OutputNode;

import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;

/**
 * Visitor to remove the class attribute from a transaction
 * element upon serialization and to add the class back
 * upon deserialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionTypeVisitor implements Visitor {
	
	/**
	 * The transaction class name
	 */
	private final String className;
	
	/**
	 * Constructs a new visitor to remove transaction class
	 * names when serializing. When deserializing, the given
	 * transaction class name is used.
	 * 
	 * @param className transaction class name to use for
	 *                   deserialization
	 */
	public TransactionTypeVisitor(String className)
	{
		this.className = className;
	}
	
	/**
	 * Adds the class attribute to transaction elements.
	 */
	@Override
    public void read(Type arg0, NodeMap<InputNode> arg1) throws Exception
    {
		if (Transaction.class.isAssignableFrom(arg0.getType()))
		{
			arg1.put("class", className);
		}
    }

	/**
	 * Removes the class attribute from transaction elements.
	 */
	@Override
    public void write(Type arg0, NodeMap<OutputNode> arg1) throws Exception
    {
		if (Transaction.class.isAssignableFrom(arg0.getType()))
		{
			arg1.remove("class");
		}
    }
	
}
