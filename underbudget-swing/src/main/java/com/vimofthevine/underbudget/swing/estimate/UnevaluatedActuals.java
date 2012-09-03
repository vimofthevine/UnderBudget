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

import com.vimofthevine.underbudget.core.assignment.ActualFigure;
import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * Actual figures source to be used until a real
 * actual figures source is provided.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class UnevaluatedActuals implements ActualFigures {
	
	/**
	 * Unevaluated commodity value
	 */
	private final CashCommodity unevaluated;
	
	/**
	 * Constructs a new unevaluated actuals source.
	 * 
	 * @param currency currency to be used
	 */
	UnevaluatedActuals(Currency currency)
	{
		unevaluated = Commodity.zero(currency);
	}

	@Override
	public ActualFigure getActual(Estimate estimate)
	{
		return new ActualFigure() {
			@Override
            public CashCommodity getAmount()
            {
				return unevaluated;
            }
		};
	}

}
