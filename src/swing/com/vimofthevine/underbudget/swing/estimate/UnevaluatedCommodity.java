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

package com.vimofthevine.underbudget.swing.estimate;

import java.util.Currency;
import java.util.Locale;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Number;

/**
 * Implementation of a cash commodity to represent an
 * unevaluated amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class UnevaluatedCommodity implements CashCommodity {

	@Override
	public Currency getCurrency()
	{
		return Currency.getInstance(Locale.getDefault());
	}

	@Override
	public Number getValue()
	{
		return new Number() {
			@Override
            public int compareTo(Number o) { return 0; }
			@Override
            public Number negate() { return this; }
			@Override
            public Number increaseBy(Number increase) { return this; }
			@Override
            public Number decreaseBy(Number decrease) { return this; }
			@Override
            public boolean within(float percentage, Number that) { return false; }
			@Override
            public boolean isNegative() { return false; }
			@Override
            public boolean isZero() { return true; }
			@Override
            public float floatValue() { return 0; }
			@Override
            public String asString() { return "0"; }
		};
	}

	@Override
	public CashCommodity negate()
	{
		return this;
	}

	@Override
	public String formatAsString()
	{
		return "";
	}
	
	@Override
	public String toString()
	{
		return formatAsString();
	}

}
