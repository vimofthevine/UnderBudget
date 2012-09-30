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

package com.vimofthevine.underbudget.swing.session.source;

import java.awt.Frame;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.source.aes.AesEncryptedFileWizard;
import com.vimofthevine.underbudget.swing.session.source.xml.BudgetXmlFileWizard;

/**
 * Wizard for selecting a budget source (e.g., file)
 * to be opened or for a source in which an existing
 * budget will be stored.
 * <p>
 * The wizard is created once per application, and
 * acts as a global service provider. When any part
 * of the application needs to prompt the user for
 * a budget source, an event is to be posted to any
 * event bus for which the wizard has been registered.
 * The selection request event contains a callback
 * method for notifying the original requestor of
 * the selected budget source.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceSelectionWizard {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetSourceSelectionWizard.class.getName());
	
	/**
	 * Internal event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Constructs a new budget source selection wizard.
	 * 
	 * @param parent application window
	 * @param prefs  user preferences
	 */
	public BudgetSourceSelectionWizard(Frame parent,
		UserPreferences prefs)
	{
		window = parent;
		preferences = prefs;
		
		eventBus = new EventBus("Budget source selection wizard event bus");
		// This is where plugin budget sources would be used
		eventBus.register(new AesEncryptedFileWizard());
		eventBus.register(new BudgetXmlFileWizard());
	}
	
	@Subscribe
	public void select(SelectSource event)
	{
		logger.log(Level.FINE, "Prompting for type of source to be selected");
		new SourceTypeSelectionDialog(window, this, event);
	}
	
	/**
	 * Upon selection of a source type, sends an event to trigger
	 * the appropriate wizard to prompt for the source details.
	 * 
	 * @param type  source type to be opened/saved
	 * @param event select budget source request event
	 */
	void typeSelected(SourceType type, SelectSource event)
	{
		logger.log(Level.FINER, type + " type has been selected");
		eventBus.post(new SelectSourceForAction(event, type,
			preferences, window));
	}

}
