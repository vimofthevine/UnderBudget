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

package com.vimofthevine.underbudget.core.budget.assignment;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;

import com.vimofthevine.underbudget.core.budget.estimate.AnalyzableEstimate;
import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;

/**
 * An <code>AssignmentRule</code> is the criterion that must
 * be met for a transaction to be associated, or assigned, to
 * an estimate.
 * <p>
 * <code>AssignmentRules</code> are user-defined on a per-estimate
 * basis.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="rule")
public class UserAssignmentRule implements AssignmentRule {
	
	/**
	 * Transaction field which is to be used
	 * when determining if a transaction meets
	 * the rule criterion
	 */
	@Element
	protected TransactionField field;
	
	/**
	 * Operation type to be used when
	 * comparing the rule text against
	 * the transaction field
	 */
	@Element
	protected ComparisonOperator operator;
	
	/**
	 * Rule text to be compared against
	 * a transaction field
	 */
	@Element
	protected String value;
	
	/**
	 * Estimate to which matching transactions
	 * will be assigned
	 */
	@Element
	protected AnalyzableEstimate estimate;
	
	/**
	 * Constructs an assignment rule with default criterion,
	 * for which no transaction will meet
	 */
	public UserAssignmentRule()
	{
		this.field    = TransactionField.NONE;
		this.operator = ComparisonOperator.NONE;
		this.value    = "";
		this.estimate = null;
	}
	
	/**
	 * Constructs an assignment rule for a generic text
	 * 
	 * @param text     comparison text
	 * @param estimate estimate to which matching transactions
	 *                  will be assigned
	 */
	public UserAssignmentRule(String text, AnalyzableEstimate estimate)
	{
		field    = TransactionField.ANY;
		operator = ComparisonOperator.CONTAINS;
		value    = text;
		this.estimate = estimate;
	}
	
	/**
	 * Constructs an assignment rule with the given
	 * criterion
	 * 
	 * @param field    comparison field
	 * @param operator comparison operator
	 * @param text     comparison text
	 * @param estimate estimate to which matching transactions
	 *                  will be assigned
	 */
	public UserAssignmentRule(TransactionField field, ComparisonOperator operator,
		String text, AnalyzableEstimate estimate)
	{
		this.field    = field;
		this.operator = operator;
		this.value    = text;
		this.estimate = estimate;
	}
	
	/**
	 * Checks if a given transaction meets the
	 * criteria for this assignment rule, and
	 * assigns the transaction to the estimate.
	 * 
	 * @param transaction transaction to be compared
	 * @return <code>true</code> if the transaction matches this
	 *          rule's criterion, else <code>false</code>
	 */
	@Override
	public boolean matches(AnalyzableTransaction transaction)
	{
		boolean matches = compare(transaction.getFieldText(field),
			operator, value);
		
		if (matches)
		{
			estimate.assignTransaction(transaction);
		}
		
		return matches;
	}
	
	@Override
	public String formatAsString()
	{
		return field.toString().toLowerCase() + " "
			+ operator.toString().toLowerCase() + " "
			+ value + " to " + estimate.toString();
	}
	
	/**
	 * Compares two strings according to the specified
	 * comparison operator
	 * 
	 * @param lhs     text to be compared
	 * @param operand comparison operator type
	 * @param rhs     text to be matched against
	 * @return <code>true</code> if the <code>lhs</code> text
	 *          matches the <code>rhs</code> text according to
	 *          the specified comparison operator
	 */
	protected boolean compare(String lhs, ComparisonOperator operand, String rhs)
	{
		switch (operand)
		{
			case BEGINS_WITH:
				return lhs.startsWith(rhs);
			case EQUALS:
				return lhs.equalsIgnoreCase(rhs);
			case EQUALS_CASE:
				return lhs.equals(rhs);
			case CONTAINS:
				return lhs.contains(rhs);
			case ENDS_WITH:
				return lhs.endsWith(rhs);
			default:
				return false;
		}
	}
	
	/**
	 * Returns the transaction field of this rule
	 * 
	 * @return transaction field
	 * @deprecated
	 */
	public TransactionField getField()
	{
		return field;
	}
	
	/**
	 * Returns the comparison operator of this rule
	 * 
	 * @return comparison operator
	 * @deprecated
	 */
	public ComparisonOperator getOperator()
	{
		return operator;
	}
	
	/**
	 * Returns the comparison text of this rule
	 * 
	 * @return comparison text
	 * @deprecated
	 */
	public String getText()
	{
		return value;
	}
	
	/**
	 * Checks if all parameters of a given rule are
	 * the same
	 * 
	 * @param object assignment rule to compare
	 * @return true if the rules are the same
	 */
	@Override
	public boolean equals(Object object)
	{
		if (object instanceof UserAssignmentRule)
		{
			UserAssignmentRule rule = (UserAssignmentRule) object;
			
			return (field.equals(rule.field))
				&& (operator.equals(rule.operator))
				&& (value.equals(rule.value));
		}
		else
			return super.equals(object);
	}
	
	@Override
	public int hashCode()
	{
		int result = 17;
		result = 31 * result + field.hashCode();
		result = 31 * result + operator.hashCode();
		result = 31 * result + value.hashCode();
		return result;
	}
	
	@Override
	public String toString()
	{
		return field + " " + operator + " " + value;
	}

}
