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
	 * @param model   application menu presentation model.
	 * @param toolBar application tool bar
	 */
	public ApplicationToolBar(ApplicationMenuModel model,
		JToolBar toolBar)
	{
		toolBar.add(new JButton(model.getAction(MenuAction.OPEN_SESSION)));
		toolBar.add(new JButton(model.getAction(MenuAction.OPEN_SESSION)));
		toolBar.add(new JButton(model.getAction(MenuAction.SAVE_SESSION)));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(model.getAction(MenuAction.BUDGET_DISPLAY)));
		toolBar.add(new JButton(model.getAction(MenuAction.EDIT_ESTIMATES)));
		toolBar.add(new JButton(model.getAction(MenuAction.ASSIGNMENT_RULES)));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(model.getAction(MenuAction.IMPORT_TRANSACTIONS)));
		toolBar.add(new JButton(model.getAction(MenuAction.ASSIGN_TRANSACTIONS)));
		toolBar.add(new JButton(model.getAction(MenuAction.CALCULATE_BALANCES)));
		toolBar.add(new JSeparator());
		toolBar.add(new JButton(model.getAction(MenuAction.ESTIMATE_PROGRESS)));
		toolBar.add(new JButton(model.getAction(MenuAction.BALANCE_IMPACT)));
		toolBar.add(new JButton(model.getAction(MenuAction.IMPORTED_TRANSACTIONS)));
	}

}
