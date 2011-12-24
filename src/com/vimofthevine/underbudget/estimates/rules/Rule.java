package com.vimofthevine.underbudget.estimates.rules;

import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.TransactionField;

/**
 * Representation of a user-defined rule for 
 * matching transactions against an estimate
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Rule {
	
	/**
	 * Transaction field which is to be used
	 * when determining if a transaction meets
	 * the rule criterion
	 */
	protected TransactionField field;
	
	/**
	 * Operation type to be used when
	 * comparing the rule text against
	 * the transaction field
	 */
	protected ComparisonOperator operator;
	
	/**
	 * Rule text to be compared against
	 * a transaction field
	 */
	protected String text;
	
	/**
	 * Default constructor
	 */
	public Rule()
	{
		this.field    = TransactionField.NONE;
		this.operator = ComparisonOperator.NONE;
		this.text     = "";
	}
	
	/**
	 * Class constructor, specifying the rule
	 * parameters
	 * 
	 * @param field    comparison field
	 * @param operator comparison operator
	 * @param text     comparison text
	 */
	public Rule(TransactionField field, ComparisonOperator operator, String text)
	{
		this.field    = field;
		this.operator = operator;
		this.text     = text;
	}
	
	/**
	 * Checks if a given transaction meets the
	 * criteria for this estimate match rule
	 * 
	 * @param transaction transaction to be compared
	 * @return true if the transaction matches this
	 *         rule's criterion, else false
	 */
	public boolean matches(Transaction transaction)
	{
		return compare(transaction.getFieldText(field),
			operator, text);
	}
	
	/**
	 * Compares two strings according to the specified
	 * comparison operator
	 * 
	 * @param lhs     text to be compared
	 * @param operand comparison operator type
	 * @param rhs     text to be matched against
	 * @return true if the lhs text matches the rhs text
	 *         via the specified comparison operator
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
	 * Returns the comparison text of this rule
	 * 
	 * @return comparison text
	 */
	public String getText()
	{
		return text;
	}

}
