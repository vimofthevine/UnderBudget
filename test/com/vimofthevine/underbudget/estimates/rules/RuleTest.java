package com.vimofthevine.underbudget.estimates.rules;

import static org.junit.Assert.*;

import org.junit.Test;

import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.TransactionField;

/**
 * Unit test case for the match rule class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleTest {

	/**
	 * Verifies that the correct field of the transaction
	 * is used for comparison
	 */
	@Test
	public void testMatches()
	{
		Rule rule = new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS, "junit");
		Transaction transaction = new StubTransaction(TransactionField.MEMO, "junit");
		assertTrue(rule.matches(transaction));
	}

	/**
	 * Verifies the text comparisons according
	 * to the different comparison operators
	 */
	@Test
	public void testCompare()
	{
		Rule rule = new Rule();
		String compare = "text to compare";
		
		assertTrue(rule.compare(compare, ComparisonOperator.BEGINS_WITH, compare));
		assertTrue(rule.compare(compare, ComparisonOperator.BEGINS_WITH, "tex"));
		assertFalse(rule.compare(compare, ComparisonOperator.BEGINS_WITH, "other"));
		
		assertTrue(rule.compare(compare, ComparisonOperator.EQUALS, compare));
		assertTrue(rule.compare(compare, ComparisonOperator.EQUALS, "teXt To COMpare"));
		assertFalse(rule.compare(compare, ComparisonOperator.EQUALS, "teXt To COMpar"));
		
		assertTrue(rule.compare(compare, ComparisonOperator.EQUALS_CASE, compare));
		assertTrue(rule.compare(compare, ComparisonOperator.EQUALS_CASE, "text to compare"));
		assertFalse(rule.compare(compare, ComparisonOperator.EQUALS_CASE, "teXt To COMpare"));
		
		assertTrue(rule.compare(compare, ComparisonOperator.CONTAINS, compare));
		assertTrue(rule.compare(compare, ComparisonOperator.CONTAINS, "comp"));
		assertFalse(rule.compare(compare, ComparisonOperator.CONTAINS, "other"));
		
		assertTrue(rule.compare(compare, ComparisonOperator.ENDS_WITH, compare));
		assertTrue(rule.compare(compare, ComparisonOperator.ENDS_WITH, "are"));
		assertFalse(rule.compare(compare, ComparisonOperator.ENDS_WITH, "other"));
		
		assertFalse(rule.compare(compare, ComparisonOperator.NONE, compare));
	}
	
	/**
	 * Stub transaction
	 */
	private class StubTransaction extends Transaction {
		
		TransactionField expectedField;
		
		String fieldText;
		
		/**
		 * Class constructor, specifying a field and
		 * the text value for that field
		 * 
		 * @param field stub field
		 * @param text  stub field text
		 */
		public StubTransaction(TransactionField field, String text)
		{
			expectedField = field;
			fieldText     = text;
		}
		
		/**
		 * Verifies that the expected field is the requested
		 * field, failing the test if otherwise
		 */
		@Override
		public String getFieldText(TransactionField field) {
			if (field.equals(expectedField))
				return fieldText;
			else
			{
				fail("Expected field " + expectedField
					+ " to be requested but received request for " + field);
				return "";
			}
		}
		
	}

}
