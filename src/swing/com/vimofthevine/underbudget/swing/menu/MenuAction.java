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

import com.vimofthevine.underbudget.swing.AboutDisplayEvent;
import com.vimofthevine.underbudget.swing.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.swing.session.content.SessionContent;
import com.vimofthevine.underbudget.swing.session.events.CloseSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.CreateSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.OpenSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveAsSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionContentEvent;
import com.vimofthevine.underbudget.swing.session.events.UpdateTemplateEvent;

/**
 * Enumeration type for menu actions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
enum MenuAction {
	// Session actions
	CREATE_SESSION("New", KeyEvent.VK_N, new CreateSessionEvent()),
	OPEN_SESSION("Open...", KeyEvent.VK_O, new OpenSessionEvent()),
	SAVE_SESSION("Save", KeyEvent.VK_S, new SaveSessionEvent()),
	SAVE_SESSION_AS("Save as...", KeyEvent.VK_A, new SaveAsSessionEvent()),
	SAVE_SESSION_AS_TEMPLATE("Save as template", KeyEvent.VK_T, new UpdateTemplateEvent()),
	CLOSE_SESSION("Close", KeyEvent.VK_C, new CloseSessionEvent()),
	// Analysis actions
	EXPORT_RESULTS("Export...", KeyEvent.VK_E, null),
	IMPORT_TRANSACTIONS("Import transactions...", KeyEvent.VK_I, null),
	IMPORT_TRANSACTIONS_FROM("Import transactions from...", KeyEvent.VK_F, null),
	ASSIGN_TRANSACTIONS("Assign transactions", KeyEvent.VK_A, null),
	CALCULATE_BALANCES("Calculate balances", KeyEvent.VK_C, null),
	// Display actions
	BUDGET_DISPLAY("Budget", KeyEvent.VK_B, new SessionContentEvent(SessionContent.BUDGET)),
	ASSIGNMENT_RULES("Assignment Rules", KeyEvent.VK_R, new SessionContentEvent(SessionContent.ASSIGNMENT_RULES)),
	EDIT_ESTIMATES("Estimates", KeyEvent.VK_E, new SessionContentEvent(SessionContent.ESTIMATE_PROGRESS)),
	ESTIMATE_PROGRESS("Estimate progress", KeyEvent.VK_P, new SessionContentEvent(SessionContent.ESTIMATE_PROGRESS)),
	BALANCE_IMPACT("Balance impact", KeyEvent.VK_B, new SessionContentEvent(SessionContent.BALANCE_IMPACT)),
	IMPORTED_TRANSACTIONS("Imported transactions", KeyEvent.VK_T, new SessionContentEvent(SessionContent.IMPORTED_TRANSACTIONS)),
	ANALYSIS_SUMMARY("Analysis summary", KeyEvent.VK_S, new SessionContentEvent(SessionContent.ANALYSIS_SUMMARY)),
	PREFERENCES("Preferences", KeyEvent.VK_P, null),
	ABOUT("About", KeyEvent.VK_A, new AboutDisplayEvent()),
	// Application actions
	NEW_WINDOW("New Window", KeyEvent.VK_W, null),
	SHUTDOWN("Exit", KeyEvent.VK_X, new ApplicationShutdownEvent());
	
	/**
	 * Menu action name
	 */
	private final String name;
	
	/**
	 * Menu action mnemonic
	 */
	private final int mnemonic;
	
	/**
	 * Menu action event
	 */
	private final Object event;
	
	/**
	 * Constructs a new menu action enumeration.
	 * 
	 * @param name     menu action name
	 * @param mnemonic menu action mnemonic
	 * @param event    menu action event
	 */
	private MenuAction(String name, int mnemonic, Object event)
	{
		this.name = name;
		this.mnemonic = mnemonic;
		this.event = event;
	}
	
	/**
	 * Returns the display name for this
	 * menu action.
	 * 
	 * @return menu action name
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 * Returns the keyboard mnemonic key
	 * for this menu action.
	 * 
	 * @return mnemonic key
	 */
	public int getMnemonic()
	{
		return mnemonic;
	}
	
	/**
	 * Returns the event to be posted
	 * in response to this menu action.
	 * 
	 * @return menu action event
	 */
	public Object getEvent()
	{
		return event;
	}
	
}
