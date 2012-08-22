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

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * Total actual expense or income in contrast
 * to an estimated expense or income total.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface ActualFigure {
	
	/**
	 * Returns the sum actual expense or
	 * income amount.
	 * 
	 * @return actual expense or income amount
	 */
	public Currency getAmount();

}
