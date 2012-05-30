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

package com.vimofthevine.underbudget.core.budget.estimate.impact;

import static org.junit.Assert.*;

import org.junit.Test;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Unit test case for the ExpenseImpact class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ExpenseImpactTest {

	/**
	 * Verifies that expense impacts deduct
	 * from the balance.
	 */
	@Test
	public final void testApply()
	{
		ExpenseImpact impact = new ExpenseImpact();
		CurrencyFactory factory = new CurrencyFactory("USD");
		Currency balance = factory.newCurrencyInstance("100.00");
		Currency expense = factory.newCurrencyInstance("50.00");
		
		impact.apply(expense, balance);
		assertEquals(factory.newCurrencyInstance("50.00"), balance);
	}

}
