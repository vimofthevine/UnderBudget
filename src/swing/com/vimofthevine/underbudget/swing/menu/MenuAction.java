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

import com.vimofthevine.underbudget.swing.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.swing.content.ContentDisplay;
import com.vimofthevine.underbudget.swing.content.ContentDisplayEvent;
import com.vimofthevine.underbudget.swing.session.CloseSessionEvent;
import com.vimofthevine.underbudget.swing.session.CreateSessionEvent;
import com.vimofthevine.underbudget.swing.session.OpenSessionEvent;
import com.vimofthevine.underbudget.swing.session.SaveAsSessionEvent;
import com.vimofthevine.underbudget.swing.session.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.UpdateTemplateEvent;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
enum MenuAction {
	CREATE_SESSION("New", KeyEvent.VK_N, new CreateSessionEvent()),
	OPEN_SESSION("Open...", KeyEvent.VK_O, new OpenSessionEvent()),
	SAVE_SESSION("Save", KeyEvent.VK_S, new SaveSessionEvent()),
	SAVE_SESSION_AS("Save as...", KeyEvent.VK_A, new SaveAsSessionEvent()),
	SAVE_SESSION_AS_TEMPLATE("Save as template", KeyEvent.VK_T, new UpdateTemplateEvent()),
	CLOSE_SESSION("Close", KeyEvent.VK_C, new CloseSessionEvent()),
	EXPORT_RESULTS("Export...", KeyEvent.VK_E, null),
	SHUTDOWN("Exit", KeyEvent.VK_X, new ApplicationShutdownEvent()),
	BUDGET_DISPLAY("Budget", KeyEvent.VK_B, new ContentDisplayEvent(ContentDisplay.EDIT_BUDGET)),
	ASSIGNMENT_RULES("Assignment Rules", KeyEvent.VK_R, new ContentDisplayEvent(ContentDisplay.ASSIGNMENT_RULES)),
	EDIT_ESTIMATES("Estimates", KeyEvent.VK_E, new ContentDisplayEvent(ContentDisplay.ESTIMATE_PROGRESS)),
	ESTIMATE_PROGRESS("Estimate progress", KeyEvent.VK_P, new ContentDisplayEvent(ContentDisplay.ESTIMATE_PROGRESS)),
	BALANCE_IMPACT("Balance impact", KeyEvent.VK_B, new ContentDisplayEvent(ContentDisplay.BALANCE_IMPACT)),
	IMPORTED_TRANSACTIONS("Imported transactions", KeyEvent.VK_T, new ContentDisplayEvent(ContentDisplay.IMPORTED_TRANSACTIONS)),
	ANALYSIS_SUMMARY("Analysis summary", KeyEvent.VK_S, new ContentDisplayEvent(ContentDisplay.ANALYSIS_SUMMARY)),
	PREFERENCES("Preferences", KeyEvent.VK_P, new ContentDisplayEvent(ContentDisplay.PREFERENCES)),
	ABOUT("About", KeyEvent.VK_A, new ContentDisplayEvent(ContentDisplay.ABOUT)),
	IMPORT_TRANSACTIONS("Import transactions...", KeyEvent.VK_I, null),
	IMPORT_TRANSACTIONS_FROM("Import transactions from...", KeyEvent.VK_F, null),
	ASSIGN_TRANSACTIONS("Assign transactions", KeyEvent.VK_A, null),
	CALCULATE_BALANCES("Calculate balances", KeyEvent.VK_C, null);
	
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
	
	public String getName()
	{
		return name;
	}
	
	public int getMnemonic()
	{
		return mnemonic;
	}
	
	public Object getEvent()
	{
		return event;
	}
	
}
