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

package com.vimofthevine.underbudget.core.currency;

/**
 * Stub currency for unit testing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ABCCurrency implements Currency {

	public ABCCurrency() { }
	
	@Override
	public void parse(String amount) { }
	
	@Override
	public Currency clone()
	{
		return new ABCCurrency();
	}

	@Override
    public int compareTo(Currency o)
    {
        return 0;
    }
	
	@Override
	public boolean within(float percent, Currency o) { return false; }

	@Override
    public Currency increaseBy(Currency increase) { return null; }

	@Override
    public Currency decreaseBy(Currency decrease) { return null; }

	@Override
    public boolean isNegative()
    {
        return false;
    }

	@Override
    public String formatAsString()
    {
        return "Type A";
    }

	@Override
    public boolean isZero()
    {
        return false;
    }
	
}
