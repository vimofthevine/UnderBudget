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

package com.vimofthevine.underbudget.swing.analysis;

import javax.swing.text.Document;

/**
 * Presentation model for views that display budget
 * analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface SummaryModel {
	
	/**
	 * Returns a document representing
	 * the budget's name.
	 * 
	 * @return budget name document
	 */
	public Document getNameDocument();

	/**
	 * Returns a document representing
	 * the budget's initial balance.
	 * 
	 * @return budget initial balance document
	 */
	public Document getInitialBalanceDocument();

	/**
	 * Returns a document representing
	 * the budget's period.
	 * 
	 * @return budget period document
	 */
	public Document getPeriodDocument();

}
