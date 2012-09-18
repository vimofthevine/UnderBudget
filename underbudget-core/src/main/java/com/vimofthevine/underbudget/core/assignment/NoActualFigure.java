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

package com.vimofthevine.underbudget.core.assignment;

import com.vimofthevine.underbudget.core.currency.CashCommodity;

/**
 * <code>ActualFigure</code> implementation to
 * encapsulate a zero commodity value.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class NoActualFigure implements ActualFigure {
	
	/**
	 * Actual figure
	 */
	private final CashCommodity actual;
	
	/**
	 * Constructs a new zero-amount actual figure.
	 * 
	 * @param zero zero commodity value
	 */
	NoActualFigure(CashCommodity zero)
	{
		actual = zero;
	}

	@Override
	public CashCommodity getAmount()
	{
		return actual;
	}

}
