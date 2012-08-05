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

package com.vimofthevine.underbudget.swing.menu;

import java.awt.event.KeyEvent;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;

public class ApplicationMenu {
	
	/**
	 * Constructs a new application menu.
	 * 
	 * @param model   application menu presentation model
	 * @param menuBar application menu
	 */
	public ApplicationMenu(ApplicationMenuModel model,
		JMenuBar menuBar)
	{
		JMenuBar menu = menuBar;
		
		// Create "file" menu
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic(KeyEvent.VK_F);
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.CREATE_SESSION)));
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.OPEN_SESSION)));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.SAVE_SESSION)));
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.SAVE_SESSION_AS)));
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.SAVE_SESSION_AS_TEMPLATE)));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.EXPORT_RESULTS)));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.CLOSE_SESSION)));
		fileMenu.add(new JMenuItem(model.getAction(MenuAction.SHUTDOWN)));
		
		// Create "edit" menu
		JMenu editMenu = new JMenu("Edit");
		editMenu.setMnemonic(KeyEvent.VK_E);
		editMenu.add(new JMenuItem(model.getAction(MenuAction.BUDGET_DISPLAY)));
		editMenu.add(new JMenuItem(model.getAction(MenuAction.EDIT_ESTIMATES)));
		editMenu.add(new JMenuItem(model.getAction(MenuAction.ASSIGNMENT_RULES)));
		editMenu.add(new JSeparator());
		editMenu.add(new JMenuItem(model.getAction(MenuAction.PREFERENCES)));
		
		// Create "analyze" menu
		JMenu analyzeMenu = new JMenu("Analyze");
		analyzeMenu.setMnemonic(KeyEvent.VK_A);
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.IMPORT_TRANSACTIONS)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.IMPORT_TRANSACTIONS_FROM)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.ASSIGN_TRANSACTIONS)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.CALCULATE_BALANCES)));
		analyzeMenu.add(new JSeparator());
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.ANALYSIS_SUMMARY)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.ESTIMATE_PROGRESS)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.BALANCE_IMPACT)));
		analyzeMenu.add(new JMenuItem(model.getAction(MenuAction.IMPORTED_TRANSACTIONS)));
		
		// Create "window" menu
		JMenu windowMenu = new JMenu("Window");
		windowMenu.setMnemonic(KeyEvent.VK_W);
		windowMenu.addMenuListener(new WindowMenuBuilder(model));
		
		// Create "help" menu
		JMenu helpMenu = new JMenu("Help");
		helpMenu.setMnemonic(KeyEvent.VK_H);
		helpMenu.add(new JMenuItem(model.getAction(MenuAction.ABOUT)));
		
		// Build menu
		menu.add(fileMenu);
		menu.add(editMenu);
		menu.add(analyzeMenu);
		menu.add(windowMenu);
		menu.add(helpMenu);
	}
	
}
