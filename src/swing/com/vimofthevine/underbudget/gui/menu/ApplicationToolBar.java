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

package com.vimofthevine.underbudget.gui.menu;

import javax.swing.JButton;
import javax.swing.JSeparator;
import javax.swing.JToolBar;

public class ApplicationToolBar {
	
	/**
	 * Constructs a new application tool bar.
	 * 
	 * @param presentationModel application menu presentation model.
	 * @param toolBar           application tool bar
	 */
	public ApplicationToolBar(ApplicationMenuModel presentationModel,
		JToolBar toolBar)
	{
		toolBar.add(new JButton(presentationModel.getFileNewAction()));
		toolBar.add(new JButton(presentationModel.getFileOpenAction()));
		toolBar.add(new JButton(presentationModel.getFileSaveAction()));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(presentationModel.getEditBudgetAction()));
		toolBar.add(new JButton(presentationModel.getEditEstimatesAction()));
		toolBar.add(new JButton(presentationModel.getEditAssignmentRulesAction()));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(presentationModel.getAnalyzeImportAction()));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(presentationModel.getAnalyzeEstimateProgressAction()));
		toolBar.add(new JButton(presentationModel.getAnalyzeBalanceImpactAction()));
		toolBar.add(new JButton(presentationModel.getAnalyzeImportedTransactionsAction()));
	}

}
