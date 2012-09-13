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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.net.URL;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.swing.DisplayAboutEvent;
import com.vimofthevine.underbudget.swing.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.swing.DisplayPreferencesEvent;
import com.vimofthevine.underbudget.swing.OpenNewWindowEvent;
import com.vimofthevine.underbudget.swing.analysis.CalculateBalancesEvent;
import com.vimofthevine.underbudget.swing.assignment.events.AssignTransactionsEvent;
import com.vimofthevine.underbudget.swing.export.ExportResultsEvent;
import com.vimofthevine.underbudget.swing.session.content.SessionContent;
import com.vimofthevine.underbudget.swing.session.events.CloseSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.CreateSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.OpenSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionAsEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionContentEvent;
import com.vimofthevine.underbudget.swing.session.events.UpdateTemplateEvent;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsEvent;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsFromEvent;

/**
 * Enumeration type for menu actions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
enum MenuAction {
	// Session actions
	CREATE_SESSION("New", "Create a new budget based on the template budget",
		"new.png", KeyEvent.VK_N,
		KeyStroke.getKeyStroke(KeyEvent.VK_N, KeyEvent.CTRL_DOWN_MASK),
		new CreateSessionEvent()),
		
	OPEN_SESSION("Open...", "Open an existing budget",
		"open.png", KeyEvent.VK_O,
		KeyStroke.getKeyStroke(KeyEvent.VK_O, KeyEvent.CTRL_DOWN_MASK),
		new OpenSessionEvent()),
		
	SAVE_SESSION("Save", "Save changes",
		"save.png", KeyEvent.VK_S,
		KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_DOWN_MASK),
		new SaveSessionEvent()),
		
	SAVE_SESSION_AS("Save as...", "Save the current budget to a new location",
		"save-as.png", KeyEvent.VK_A, new SaveSessionAsEvent()),
		
	SAVE_SESSION_AS_TEMPLATE("Save as template", "Update the template budget with the current budget",
		"template.png", KeyEvent.VK_T, new UpdateTemplateEvent()),
		
	CLOSE_SESSION("Close", "Close the current budget",
		"close.png", KeyEvent.VK_C,
		KeyStroke.getKeyStroke(KeyEvent.VK_W, KeyEvent.CTRL_DOWN_MASK),
		new CloseSessionEvent()),
		
	// Analysis actions
	EXPORT_RESULTS("Export...", "Export analysis results",
		"export.png", KeyEvent.VK_E,
		KeyStroke.getKeyStroke(KeyEvent.VK_X, KeyEvent.CTRL_DOWN_MASK),
		new ExportResultsEvent()),
		
	IMPORT_TRANSACTIONS("Import transactions...", "Import transactions from previous source",
		"import.png", KeyEvent.VK_I,
		KeyStroke.getKeyStroke(KeyEvent.VK_I, KeyEvent.CTRL_DOWN_MASK),
		new ImportTransactionsEvent()),
		
	IMPORT_TRANSACTIONS_FROM("Import transactions from...", "Import transactions from selected source",
		"import-from.png", KeyEvent.VK_F, new ImportTransactionsFromEvent()),
		
	ASSIGN_TRANSACTIONS("Assign transactions", "Assign imported transactions",
		"assign.png", KeyEvent.VK_A,
		KeyStroke.getKeyStroke(KeyEvent.VK_A, KeyEvent.CTRL_DOWN_MASK),
		new AssignTransactionsEvent()),
		
	CALCULATE_BALANCES("Calculate balances", "Calculate ending balances",
		"calc.png", KeyEvent.VK_C,
		KeyStroke.getKeyStroke(KeyEvent.VK_C, KeyEvent.CTRL_DOWN_MASK),
		new CalculateBalancesEvent()),
		
	// Display actions
	BUDGET_DISPLAY("Budget", "Modify budget settings",
		"budget.png", KeyEvent.VK_B,
		KeyStroke.getKeyStroke(KeyEvent.VK_B, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.BUDGET)),
		
	ASSIGNMENT_RULES("Assignment Rules", "Modify assignment rules",
		"rules.png", KeyEvent.VK_R,
		KeyStroke.getKeyStroke(KeyEvent.VK_R, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.ASSIGNMENT_RULES)),
		
	EDIT_ESTIMATES("Estimates", "Modify estimates",
		"estimates.png", KeyEvent.VK_E,
		KeyStroke.getKeyStroke(KeyEvent.VK_E, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.ESTIMATE_PROGRESS)),
		
	ESTIMATE_PROGRESS("Estimate progress", "View estimate progress",
		"progress.png", KeyEvent.VK_P,
		KeyStroke.getKeyStroke(KeyEvent.VK_P, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.ESTIMATE_PROGRESS)),
		
	BALANCE_IMPACT("Balance impact", "View estimate impacts on ending balances",
		"impact.png", KeyEvent.VK_B,
		KeyStroke.getKeyStroke(KeyEvent.VK_L, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.BALANCE_IMPACT)),
		
	IMPORTED_TRANSACTIONS("Imported transactions", "View imported transactions",
		"transactions.png", KeyEvent.VK_T,
		KeyStroke.getKeyStroke(KeyEvent.VK_T, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.IMPORTED_TRANSACTIONS)),
		
	ANALYSIS_SUMMARY("Analysis summary", "View analysis results",
		"summary.png", KeyEvent.VK_S,
		KeyStroke.getKeyStroke(KeyEvent.VK_Y, KeyEvent.CTRL_DOWN_MASK),
		new SessionContentEvent(SessionContent.ANALYSIS_SUMMARY)),
		
	PREFERENCES("Preferences", "Edit application preferences",
		"preferences.png", KeyEvent.VK_P, new DisplayPreferencesEvent()),
		
	ABOUT("About", "View information about the application",
		"about.png", KeyEvent.VK_A, new DisplayAboutEvent()),
		
	// Application actions
	NEW_WINDOW("New Window", "Open a new window",
		"new-window.png", KeyEvent.VK_W, new OpenNewWindowEvent()),
		
	SHUTDOWN("Exit", "Shut down application",
		"exit.png", KeyEvent.VK_X,
		KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.CTRL_DOWN_MASK),
		new ApplicationShutdownEvent());
	
	/**
	 * Menu action name
	 */
	private final String name;
	
	/**
	 * Menu action description
	 */
	private final String description;
	
	/**
	 * Menu action icon name
	 */
	private final String icon;
	
	/**
	 * Menu action mnemonic
	 */
	private final int mnemonic;
	
	/**
	 * Menu action accelerator (shortcut key)
	 */
	private final KeyStroke accelerator;
	
	/**
	 * Menu action event
	 */
	private final Object event;
	
	/**
	 * Constructs a new menu action enumeration.
	 * 
	 * @param name        menu action name
	 * @param description menu action description
	 * @param icon        menu action large icon
	 * @param mnemonic    menu action mnemonic
	 * @param event       menu action event
	 */
	private MenuAction(String name, String description,
		String icon, int mnemonic, Object event)
	{
		this(name, description, icon, mnemonic, null, event);
	}
	
	/**
	 * Constructs a new menu action enumeration.
	 * 
	 * @param name        menu action name
	 * @param description menu action description
	 * @param icon        menu action large icon
	 * @param mnemonic    menu action mnemonic
	 * @param shortcut    menu action shortcut key stroke
	 * @param event       menu action event
	 */
	private MenuAction(String name, String description,
		String icon, int mnemonic, KeyStroke shortcut,
		Object event)
	{
		this.name = name;
		this.description = description;
		this.icon = icon;
		this.mnemonic = mnemonic;
		this.accelerator = shortcut;
		this.event = event;
	}
	
	/**
	 * Creates an action model for this menu action.
	 * 
	 * @param bus event bus to use
	 */
	public Action getAction(final EventBus bus)
	{
		return new AbstractAction() {
			{
				putValue(NAME, name);
				putValue(MNEMONIC_KEY, mnemonic);
				
				if ( ! description.equals(""))
				{
					putValue(SHORT_DESCRIPTION, description);
				}
				
				if ( ! icon.equals(""))
				{
					URL smallUrl = getClass().getResource("icons/small-" + icon);
					if (smallUrl != null)
					{
						putValue(SMALL_ICON, new ImageIcon(smallUrl));
					}
					
					URL largeUrl = getClass().getResource("icons/large-" + icon);
					if (largeUrl != null)
					{
						putValue(LARGE_ICON_KEY, new ImageIcon(largeUrl));
					}
				}
				
				if (accelerator != null)
				{
					putValue(ACCELERATOR_KEY, accelerator);
				}
			}
			
			public void actionPerformed(ActionEvent actionEvent)
			{
				bus.post(event);
			}
		};
	}
	
}
