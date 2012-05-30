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

import org.junit.Test;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Unit test case for the CategoryEstimateBehavior class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CategoryEstimateBehaviorTest {

	/**
	 * Verifies that category estimates don't
	 * contribute to the estimate progress report.
	 */
	@Test
	public final void testReport()
	{
		StubDisplayableEstimate estimate = new StubDisplayableEstimate();
		StubEstimateProgressReport report = new StubEstimateProgressReport();
		CategoryEstimateBehavior behavior = new CategoryEstimateBehavior();
		CurrencyFactory factory = new CurrencyFactory("USD");
		Currency currency = factory.newCurrencyInstance("100.00");
		
		behavior.report(estimate, currency, currency, report);
		
		assertNull(report.dueDate);
		assertFalse(report.overBudget);
		assertFalse(report.underBudget);
		assertFalse(report.unbudgeted);
		assertFalse(report.progress);
	}

}
