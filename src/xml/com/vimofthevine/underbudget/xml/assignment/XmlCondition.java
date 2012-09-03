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

package com.vimofthevine.underbudget.xml.assignment;

import org.simpleframework.xml.Element;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.DefaultCondition;
import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.TransactionField;

/**
 * A <code>Condition</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlCondition implements Condition {
	
	/**
	 * Original condition
	 */
	private final Condition original;
	
	/**
	 * Transaction field to be evaluated
	 */
	@Element
	private final TransactionField field;
	
	/**
	 * Evaluation operator
	 */
	@Element
	private final EvaluationOperator operator;
	
	/**
	 * Evaluation value
	 */
	@Element
	private final String value;
	
	/**
	 * Constructs a new XML serializable
	 * condition wrapper around the given condition.
	 * 
	 * @param condition original condition
	 */
	public XmlCondition(Condition condition)
	{
		this(
			condition.getField(),
			condition.getOperator(),
			condition.getValue()
		);
	}
	
	/**
	 * Constructs a new XML serializable condition
	 * with the given evaluation parameters.
	 * 
	 * @param field    evaluation field
	 * @param operator evaluation operator
	 * @param value    evaluation value
	 */
	XmlCondition(
		@Element(name="field") TransactionField field,
		@Element(name="operator") EvaluationOperator operator,
		@Element(name="value") String value
	)
	{
		original = new DefaultCondition(field, operator, value);
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
    public boolean satisfies(String value)
    {
		return original.satisfies(value);
    }
	
	@Override
	public int hashCode()
	{
		int result = 61;
		result = result * 31 + field.hashCode();
		result = result * 31 + operator.hashCode();
		result = result * 31 + value.hashCode();
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof XmlCondition))
			return false;
		
		XmlCondition that = (XmlCondition) obj;
		return this.field.equals(that.field)
			&& this.operator.equals(that.operator)
			&& this.value.equals(that.value);
	}
	
	@Override
	public String toString()
	{
		return field + " " + operator + " " + value;
	}

}
