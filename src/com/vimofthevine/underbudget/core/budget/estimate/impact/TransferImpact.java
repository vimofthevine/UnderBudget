/*
 * 
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

import org.simpleframework.xml.Text;

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * Transfer amounts have no impact on ending balances. Thus,
 * estimates can be set up to indicate credit card payments,
 * for example.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransferImpact implements BalanceImpact {
	
	/**
	 * Balance impact type
	 */
	@Text(required=false)
	public static final String TYPE = "transfer";
	
	@Override
	public void apply(Currency amount, Currency total)
	{
		// Do nothing
	}
	
	@Override
	public int hashCode()
	{
		return 33;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof TransferImpact)
			return true;
		else
			return false;
	}
	
	@Override
	public String toString()
	{
		return "Transfer Impact";
	}

}
