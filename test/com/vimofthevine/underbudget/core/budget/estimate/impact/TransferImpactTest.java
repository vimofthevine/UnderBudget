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
 * Unit test case for the TransferImpact class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransferImpactTest {

	/**
	 * Verifies that transfer impact have
	 * no affect on the balance.
	 */
	@Test
	public final void testApply()
	{
		TransferImpact impact = new TransferImpact();
		CurrencyFactory factory = new CurrencyFactory("USD");
		Currency balance = factory.newCurrencyInstance("100.00");
		Currency expected = factory.newCurrencyInstance("100.00");
		
		impact.apply(factory.newCurrencyInstance("75.00"), balance);
		assertEquals(expected, balance);
		
		impact.apply(factory.newCurrencyInstance("0.00"), balance);
		assertEquals(expected, balance);
		
		impact.apply(factory.newCurrencyInstance("-5.00"), balance);
		assertEquals(expected, balance);
	}

}
