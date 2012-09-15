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

package com.vimofthevine.underbudget.swing.transaction.wizard;

import java.awt.Frame;

/**
 * Wizard for selecting a specific transaction
 * source.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface SourceWizard {
	
	/**
	 * Launches the wizard for selecting the
	 * specific transaction source.
	 * 
	 * @param window application main window
	 * @param wizard transaction source selection wizard
	 */
	public void select(final Frame window,
		final TransactionSourceSelectionWizard wizard);

}
