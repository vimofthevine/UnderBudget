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

import java.awt.event.KeyEvent;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;

public class ApplicationMenu {
	
	private final JMenuBar menu;

	/**
	 * Constructs a new application menu.
	 * 
	 * @param presentationModel application menu presentation model
	 * @param menuBar           application menu
	 */
	public ApplicationMenu(ApplicationMenuModel presentationModel,
		JMenuBar menuBar)
	{
		menu = menuBar;
		
		// Create "file" menu
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic(KeyEvent.VK_F);
		fileMenu.add(new JMenuItem(presentationModel.getFileNewAction()));
		fileMenu.add(new JMenuItem(presentationModel.getFileOpenAction()));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(presentationModel.getFileSaveAction()));
		fileMenu.add(new JMenuItem(presentationModel.getFileSaveAsAction()));
		fileMenu.add(new JMenuItem(presentationModel.getFileSaveAsTemplateAction()));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(presentationModel.getFileExportAction()));
		fileMenu.add(new JSeparator());
		fileMenu.add(new JMenuItem(presentationModel.getFileCloseAction()));
		
		// Create "edit" menu
		JMenu editMenu = new JMenu("Edit");
		editMenu.setMnemonic(KeyEvent.VK_E);
		editMenu.add(new JMenuItem(presentationModel.getEditBudgetAction()));
		editMenu.add(new JMenuItem(presentationModel.getEditEstimatesAction()));
		editMenu.add(new JMenuItem(presentationModel.getEditAssignmentRulesAction()));
		editMenu.add(new JSeparator());
		editMenu.add(new JMenuItem(presentationModel.getEditPreferencesAction()));
		
		// Create "analyze" menu
		JMenu analyzeMenu = new JMenu("Analyze");
		analyzeMenu.setMnemonic(KeyEvent.VK_A);
		analyzeMenu.add(new JMenuItem(presentationModel.getAnalyzeImportAction()));
		analyzeMenu.add(new JSeparator());
		analyzeMenu.add(new JMenuItem(presentationModel.getAnalyzeEstimateProgressAction()));
		analyzeMenu.add(new JMenuItem(presentationModel.getAnalyzeBalanceImpactAction()));
		analyzeMenu.add(new JMenuItem(presentationModel.getAnalyzeImportedTransactionsAction()));
		
		// Create "window" menu
		JMenu windowMenu = new JMenu("Window");
		windowMenu.setMnemonic(KeyEvent.VK_W);
		
		// Create "help" menu
		JMenu helpMenu = new JMenu("Help");
		helpMenu.setMnemonic(KeyEvent.VK_H);
		helpMenu.add(new JMenuItem(presentationModel.getHelpAboutAction()));
		
		// Build menu
		menu.add(fileMenu);
		menu.add(editMenu);
		menu.add(analyzeMenu);
		menu.add(windowMenu);
		menu.add(helpMenu);
	}
	
}
