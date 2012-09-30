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

package com.vimofthevine.underbudget.swing.session.source;

import com.vimofthevine.underbudget.core.budget.source.BudgetSourceFactory;

/**
 * Interface for events to be posted to the event
 * bus to request the budget source selection
 * wizard to initiate a source selection.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface SelectSource {
	
	/**
	 * Whether the selected source is going to be
	 * opened or if it will be used to store an
	 * existing budget from another source.
	 * 
	 * @return <code>true</code> if the selected source
	 *         will be opened, else <code>false</code>
	 */
	public boolean isOpenRequest();
	
	/**
	 * Notifies the requestor of the selected budget source.
	 * 
	 * @param factory budget source factory instance
	 * @param summary source information for re-opening
	 */
	public void sourceSelected(BudgetSourceFactory factory,
		SourceSummary summary);

}
