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
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the OwedEstimateBehavior class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OwedEstimateBehaviorTest {
	
	/**
	 * Owed estimate behavior
	 */
	OwedEstimateBehavior behavior;
	
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
		behavior = new OwedEstimateBehavior(new SimpleDate(14, 1, 2012));
		estimate = new StubDisplayableEstimate();
		report = new StubEstimateProgressReport();
		currencyFactory = new CurrencyFactory("USD");
	}
	
	/**
	 * Verifies that owed estimates report as
	 * upcoming/overdue if there is no actual
	 * data against the estimate.
	 */
	@Test
	public final void testReportOverDue()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance("100.00"),
			currencyFactory.newCurrencyInstance(),
			report);
		
		assertNotNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}
	
	/**
	 * Verifies that owed estimates do not
	 * report as overdue if there is actual
	 * data against the estimate.
	 */
	@Test
	public final void testNoReport()
	{
		behavior.report(estimate,
			currencyFactory.newCurrencyInstance("100.00"),
			currencyFactory.newCurrencyInstance("75.00"),
			report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}

}
