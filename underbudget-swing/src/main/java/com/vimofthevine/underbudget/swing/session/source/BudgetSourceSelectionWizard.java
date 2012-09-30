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
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToOpenEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToSaveEvent;
import com.vimofthevine.underbudget.swing.session.source.aes.AesEncryptedFileWizard;
import com.vimofthevine.underbudget.swing.session.source.xml.BudgetXmlFileWizard;

/**
 * Wizard for selecting a budget to be opened.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceSelectionWizard {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetSourceSelectionWizard.class.getName());
	
	/**
	 * Event bus
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
	 * @param bus    event bus
	 * @param parent application window
	 * @param prefs  user preferences
	 */
	public BudgetSourceSelectionWizard(EventBus bus,
		Frame parent, UserPreferences prefs)
	{
		eventBus = bus;
		eventBus.register(this);
		
		window = parent;
		
		preferences = prefs;
		
		eventBus.register(new AesEncryptedFileWizard());
		eventBus.register(new BudgetXmlFileWizard());
	}
	
	@Subscribe
	public void selectSourceToOpen(SelectBudgetSourceToOpenEvent event)
	{
		logger.log(Level.FINE, "Prompting for source to open");
		new SourceTypeSelectionDialog(window, this, true);
	}
	
	@Subscribe
	public void selectSourceToSave(SelectBudgetSourceToSaveEvent event)
	{
		logger.log(Level.FINE, "Prompting for source to save");
		new SourceTypeSelectionDialog(window, this, false);
	}
	
	/**
	 * Upon selection of a source type, send an event to trigger
	 * the appropriate wizard to prompt for the source details.
	 * 
	 * @param type  source type to be opened/saved
	 * @param isOpen <code>true</code> if the selected source is to
	 *               be opened, else <code>false</false> if choosing
	 *               a source location to save an existing budget
	 */
	void typeSelected(SourceType type, boolean isOpen)
	{
		logger.log(Level.FINER, type + " type has been selected (open? " + isOpen);
		eventBus.post(new SelectSourceForAction( isOpen, type,
			eventBus, preferences, window));
	}

}
