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
 * Unit test case for the UnbudgetedEstimateBehavior class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UnbudgetedEstimateBehaviorTest {

	/**
	 * Unbudgeted estimate behavior
	 */
	UnbudgetedEstimateBehavior behavior;
	
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
		behavior = new UnbudgetedEstimateBehavior();
		estimate = new StubDisplayableEstimate();
		report = new StubEstimateProgressReport();
		currencyFactory = new CurrencyFactory("USD");
	}
	
	/**
	 * Verifies that any actual amount is
	 * reported as unbudgeted.
	 */
	@Test
	public final void testReportUnbudgeted()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance(),
			currencyFactory.newCurrencyInstance("12.34"), report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertTrue(report.unbudgeted);
		assertFalse(report.progress);
		
		report.unbudgeted = false;
		
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance(),
			currencyFactory.newCurrencyInstance("0.34"), report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertTrue(report.unbudgeted);
		assertFalse(report.progress);
		
		report.unbudgeted = false;
		
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance(),
			currencyFactory.newCurrencyInstance("-5.34"), report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertTrue(report.unbudgeted);
		assertFalse(report.progress);
		
		report.unbudgeted = false;
		
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance(),
			currencyFactory.newCurrencyInstance(), report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertTrue(report.unbudgeted);
		assertFalse(report.progress);
	}

}
