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

import static org.junit.Assert.*;

import org.junit.Test;

import com.vimofthevine.underbudget.core.report.BalanceCalculationReport;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;
import com.vimofthevine.underbudget.core.budget.assignment.UserAssignmentRule;
import com.vimofthevine.underbudget.core.budget.assignment.ComparisonOperator;
import com.vimofthevine.underbudget.core.budget.estimate.AnalyzableEstimate;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the AssignmentRule class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UserAssignmentRuleTest {

	/**
	 * Verifies that the correct field of the transaction
	 * is used for comparison
	 */
	@Test
	public void testMatches()
	{
		StubEstimate estimate = new StubEstimate();
		UserAssignmentRule rule = new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.EQUALS, "junit", estimate);
		AnalyzableTransaction transaction = new StubTransaction(TransactionField.MEMO, "junit");
		assertTrue(rule.matches(transaction));
	}

	/**
	 * Verifies the text comparisons according
	 * to the different comparison operators
	 */
	@Test
	public void testCompare()
	{
		UserAssignmentRule rule = new UserAssignmentRule();
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
	 * Verifies that the equals method correctly
	 * determines when a given rule is the same
	 */
	@Test
	public final void testEquals()
	{
		UserAssignmentRule base = new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.EQUALS, "junit", null);
		UserAssignmentRule equal = new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.EQUALS, "junit", null);
		UserAssignmentRule diffField = new UserAssignmentRule(TransactionField.PAYEE, ComparisonOperator.EQUALS, "junit", null);
		UserAssignmentRule diffOperator = new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.CONTAINS, "junit", null);
		UserAssignmentRule diffText = new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.EQUALS, "unit test", null);
		
		assertFalse(base.equals(null));
		assertTrue(base.equals(base)); // reflexive
		assertTrue(base.equals(equal));
		assertTrue(equal.equals(base)); // symmetric
		assertFalse(base.equals(diffField));
		assertFalse(base.equals(diffOperator));
		assertFalse(base.equals(diffText));
		assertFalse(base.equals(new StubTransaction(TransactionField.MEMO, "junit")));
		
		assertEquals(base, equal); // Generic equals
	}
	
	/**
	 * Stub estimate
	 */
	private class StubEstimate implements AnalyzableEstimate {

		@Override
        public void clearAssignedTransactions()
        {
			// Do nothing
        }

		@Override
        public void assignTransaction(AnalyzableTransaction transaction)
        {
			// Do nothing
        }

		@Override
        public void reportProgress(EstimateProgressReport report)
        {
			// Do nothing
        }

		@Override
        public void addValuesTo(Currency estimated, Currency actual,
                Currency expected, BalanceCalculationReport report)
        {
			// Do nothing
        }

	}
	
	/**
	 * Stub transaction
	 */
	private class StubTransaction implements AnalyzableTransaction {
		
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

		@Override
        public void addTransferAmountTo(Currency total)
        {
			// Do nothing
        }

		@Override
        public int compareTo(SimpleDate date)
        {
	        return 0;
        }

		@Override
        public boolean isWithin(BudgetingPeriod period)
        {
	        return false;
        }
		
	}

}
