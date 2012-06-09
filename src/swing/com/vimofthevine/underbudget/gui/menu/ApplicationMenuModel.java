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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.gui.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.gui.content.ContentDisplay;
import com.vimofthevine.underbudget.gui.content.ContentDisplayEvent;

public class ApplicationMenuModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Constructs a new application menu model.
	 * 
	 * @param bus   event bus
	 */
	public ApplicationMenuModel(EventBus bus)
	{
		eventBus = bus;
	}
	
	private Action createContentDisplayAction(final String name,
		final int mnemonic, final ContentDisplay content)
	{
		return new AbstractAction() {
			{
				putValue(NAME, name);
				putValue(MNEMONIC_KEY, mnemonic);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				eventBus.post(new ContentDisplayEvent(content));
			}
		};
	}
	
	public Action getFileNewAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "New");
				putValue(MNEMONIC_KEY, KeyEvent.VK_N);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}
	
	public Action getFileOpenAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Open...");
				putValue(MNEMONIC_KEY, KeyEvent.VK_O);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}
	
	public Action getFileSaveAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Save");
				putValue(MNEMONIC_KEY, KeyEvent.VK_S);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}

	public Action getFileSaveAsAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Save as...");
				putValue(MNEMONIC_KEY, KeyEvent.VK_A);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}

	public Action getFileSaveAsTemplateAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Save as template");
				putValue(MNEMONIC_KEY, KeyEvent.VK_T);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}
	
	public Action getFileExportAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Export...");
				putValue(MNEMONIC_KEY, KeyEvent.VK_E);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}

	public Action getFileCloseAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Exit");
				putValue(MNEMONIC_KEY, KeyEvent.VK_X);
			}
			
			public void actionPerformed(ActionEvent e)
			{
				eventBus.post(new ApplicationShutdownEvent());
			}
		};
	}
	
	public Action getEditBudgetAction()
	{
		return createContentDisplayAction("Budget", KeyEvent.VK_B,
			ContentDisplay.EDIT_BUDGET);
	}
	
	public Action getEditEstimatesAction()
	{
		return createContentDisplayAction("Estimates", KeyEvent.VK_E,
			ContentDisplay.ESTIMATE_PROGRESS);
	}
	
	public Action getEditAssignmentRulesAction()
	{
		return createContentDisplayAction("Assignment Rules", KeyEvent.VK_R,
			ContentDisplay.ASSIGNMENT_RULES);
	}
	
	public Action getEditPreferencesAction()
	{
		return createContentDisplayAction("Preferences", KeyEvent.VK_P,
			ContentDisplay.PREFERENCES);
	}
	
	public Action getAnalyzeImportAction()
	{
		return new AbstractAction() {
			{
				putValue(NAME, "Import transactions...");
				putValue(MNEMONIC_KEY, KeyEvent.VK_I);
			}
			
			public void actionPerformed(ActionEvent event)
			{
				// TODO
			}
		};
	}
	
	public Action getAnalyzeEstimateProgressAction()
	{
		return createContentDisplayAction("Estimate Progress", KeyEvent.VK_P,
			ContentDisplay.ESTIMATE_PROGRESS);
	}
	
	public Action getAnalyzeBalanceImpactAction()
	{
		return createContentDisplayAction("Balance Impact", KeyEvent.VK_B,
			ContentDisplay.BALANCE_IMPACT);
	}
	
	public Action getAnalyzeImportedTransactionsAction()
	{
		return createContentDisplayAction("Imported Transactions", KeyEvent.VK_T,
			ContentDisplay.IMPORTED_TRANSACTIONS);
	}
	
	public Action getHelpAboutAction()
	{
		return createContentDisplayAction("About", KeyEvent.VK_A,
			ContentDisplay.ABOUT);
	}
	
}
