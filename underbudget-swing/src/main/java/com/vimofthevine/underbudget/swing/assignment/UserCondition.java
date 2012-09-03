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

package com.vimofthevine.underbudget.swing.assignment;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.TransactionField;

/**
 * A condition implementation used when a user modifies
 * an existing rule condition or creates a new condition.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class UserCondition implements Condition {
	
	/**
	 * Transaction field
	 */
	private final TransactionField field;
	
	/**
	 * Evaluation operator
	 */
	private final EvaluationOperator operator;
	
	/**
	 * Evaluation value
	 */
	private final String value;
	
	/**
	 * Constructs a new, blank, rule condition.
	 */
	UserCondition()
	{
		field = TransactionField.NONE;
		operator = EvaluationOperator.NONE;
		value = "";
	}
	
	/**
	 * Constructs a new rule condition based on
	 * an existing rule, replacing the transaction field.
	 * 
	 * @param existing existing rule condition
	 * @param newField new transaction field
	 */
	UserCondition(Condition existing, TransactionField newField)
	{
		field = newField;
		operator = existing.getOperator();
		value = existing.getValue();
	}
	
	/**
	 * Constructs a new rule condition based on
	 * an existing rule, replacing the evaluation operator.
	 * 
	 * @param existing    existing rule condition
	 * @param newOperator new evaluation operator
	 */
	UserCondition(Condition existing, EvaluationOperator newOperator)
	{
		field = existing.getField();
		operator = newOperator;
		value = existing.getValue();
	}
	
	/**
	 * Constructs a new rule condition based on
	 * an existing rule, replacing the evaluation value.
	 * 
	 * @param existing existing rule condition
	 * @param newValue new evaluation value
	 */
	UserCondition(Condition existing, String newValue)
	{
		field = existing.getField();
		operator = existing.getOperator();
		value = newValue;
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
		return false;
	}

}
