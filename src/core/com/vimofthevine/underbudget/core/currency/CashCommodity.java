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

import java.util.Currency;

/**
 * Interface for cash commodities.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface CashCommodity {

	/**
	 * Returns the currency of this cash commodity.
	 * 
	 * @return cash commodity currency
	 */
	public Currency getCurrency();
	
	/**
	 * Returns the numerical value
	 * of this cash commodity.
	 * 
	 * @return numerical value of this cash commodity
	 */
	public Number getValue();
	
	/**
	 * Returns an inverse representation of
	 * this cash commodity.
	 * 
	 * @return inverse of this cash commodity
	 */
	public CashCommodity negate();
	
	/**
	 * Creates a formatted representation of
	 * this cash commodity value.
	 * 
	 * @return formatted cash commodity value
	 */
	public String formatAsString();
	
}
