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

package com.vimofthevine.underbudget.swing.currency;

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * Interface for listeners to be notified when the
 * currency changes in a currency input model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface CurrencyChangeListener {

	/**
	 * Notifies the listeners that the currency value
	 * has changed.
	 * 
	 * @param currency new currency value
	 */
	public void currencyChanged(Currency currency);
	
}
