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

package com.vimofthevine.underbudget.stubs.assignment;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.MutableAssignmentRule;
import com.vimofthevine.underbudget.core.assignment.RuleDefinition;
import com.vimofthevine.underbudget.core.assignment.TransactionField;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubAssignmentRule implements MutableAssignmentRule {
	
	private final Estimate estimate;
	
	private Condition[] conditions;
	
	public StubAssignmentRule(Estimate estimate)
	{
		this.estimate = estimate;
		conditions = new Condition[0];
	}
	
	public StubAssignmentRule(Estimate estimate, final TransactionField field,
		final EvaluationOperator operator, final String value)
	{
		this.estimate = estimate;
		conditions = new Condition[] {
			new Condition() {

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
				
			},
		};
	}

	@Override
	public RuleDefinition getDefinition()
	{
		return new RuleDefinition() {

			@Override
            public Condition[] getConditions()
            {
				return conditions;
            }
			
		};
	}

	@Override
	public boolean matches(Transaction transaction)
	{
		if (conditions.length == 0)
			return false;
		
		boolean matches = true;
		
		for (Condition condition : conditions)
		{
			matches = matches && transaction.meets(condition);
		}
		
		return matches;
	}

	@Override
	public Estimate getEstimate()
	{
		return estimate;
	}

	@Override
	public void setDefinition(RuleDefinition definition)
	{
		conditions = definition.getConditions();
	}
	
	@Override
	public String toString()
	{
		StringBuilder buffer = new StringBuilder("Rule ");
		
		for (Condition condition : conditions)
		{
			buffer.append("{");
			buffer.append(condition.getField());
			buffer.append(" ");
			buffer.append(condition.getOperator());
			buffer.append(" ");
			buffer.append(condition.getValue());
			buffer.append("}");
		}
		
		if (conditions.length == 0)
		{
			buffer.append("{None}");
		}
		
		return buffer.toString();
	}

}
