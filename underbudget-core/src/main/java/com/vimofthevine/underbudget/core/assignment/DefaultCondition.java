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

package com.vimofthevine.underbudget.core.assignment;

/**
 * Default implementation of a <code>Condition</code>
 * to encapsulate the default condition logic.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultCondition implements Condition {
	
	/**
	 * Evaluation field
	 */
	private final TransactionField field;
	
	/**
	 * Evaluation operator
	 */
	private final EvaluationOperator operator;
	
	/**
	 * Evaluation text
	 */
	private final String value;
	
	/**
	 * Constructs a new condition.
	 * 
	 * @param field    evaluation field
	 * @param operator evaluation operator
	 * @param value    evaluation text
	 */
	public DefaultCondition(TransactionField field,
		EvaluationOperator operator, String value)
	{
		this.field = field;
		this.operator = operator;
		this.value = value;
	}

	@Override
	public TransactionField getField()
	{
		return field;
	}

	@Override
	public EvaluationOperator getOperator()
	{
		return operator;
	}

	@Override
	public String getValue()
	{
		return value;
	}

	@Override
	public boolean satisfies(String that)
	{
		System.out.println("am i hitting this?");
		switch (operator)
		{
			case BEGINS_WITH:
				return that.startsWith(value);
				
			case EQUALS:
				return that.equalsIgnoreCase(value);
				
			case EQUALS_CASE:
				return that.equals(value);
				
			case CONTAINS:
				return that.contains(value);
				
			case ENDS_WITH:
				System.out.println("Checking if " + that + " begins with "
					+ value + "? " + that.endsWith(value));
				return that.endsWith(value);
				
			default:
				return false;
		}
	}

}
