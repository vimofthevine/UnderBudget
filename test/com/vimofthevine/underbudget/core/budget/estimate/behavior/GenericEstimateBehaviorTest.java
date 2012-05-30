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

package com.vimofthevine.underbudget.core.budget.estimate.behavior;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Unit test case for the GenericEstimateBehavior class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GenericEstimateBehaviorTest {
	
	/**
	 * Generic estimate behavior
	 */
	GenericEstimateBehavior behavior;
	
	/**
	 * Stub displayable estimate
	 */
	StubDisplayableEstimate estimate;
	
	/**
	 * Stub estimate progress report
	 */
	StubEstimateProgressReport report;
	
	/**
	 * Currency factory
	 */
	CurrencyFactory currencyFactory;
		
	/**
	 * Sets up the test case objects
	 */
	@Before
	public final void setUp()
	{
		behavior = new GenericEstimateBehavior();
		estimate = new StubDisplayableEstimate();
		report = new StubEstimateProgressReport();
		currencyFactory = new CurrencyFactory("USD");
	}

	/**
	 * Verifies that generic estimates report
	 * as over-budget if the actual exceeds the
	 * estimated amount.
	 */
	@Test
	public final void testReportOverBudget()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance("100.00"),
			currencyFactory.newCurrencyInstance("150.00"),
			report);
		
		assertNull(report.dueDate);
		assertTrue(report.overBudget);
		assertFalse(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}
	
	/**
	 * Verifies that generic estimates report
	 * as under-budget if the estimated exceeds
	 * the actual amount.
	 */
	@Test
	public final void testReportUnderBudget()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance("150.00"),
			currencyFactory.newCurrencyInstance("100.00"),
			report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertTrue(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}
	
	/**
	 * Verifies that generic estimates do not
	 * report as under or over-budget if the
	 * estimated is the same as the actual.
	 */
	@Test
	public final void testExactMatch()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance("100.00"),
			currencyFactory.newCurrencyInstance("100.00"),
			report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}

}
