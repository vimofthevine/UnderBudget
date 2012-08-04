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

package com.vimofthevine.underbudget.swing.session.wizard;

import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.stubs.budget.source.StubBudgetSource;

/**
 * Wizard for selecting a budget to be opened.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceSelectionWizard {

	/**
	 * Walks the user through the selection of
	 * a source for an existing budget to be opened.
	 * 
	 * @return selected budget source
	 */
	public BudgetSource getSource()
	{
		return new StubBudgetSource();
	}
	
}
