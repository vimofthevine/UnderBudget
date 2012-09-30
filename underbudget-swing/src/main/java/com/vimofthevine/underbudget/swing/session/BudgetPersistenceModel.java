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

package com.vimofthevine.underbudget.swing.session;

import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToSaveSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionAsEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToSaveEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionOpenedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionSavedEvent;
import com.vimofthevine.underbudget.swing.session.events.UpdateTemplateEvent;
import com.vimofthevine.underbudget.swing.status.StatusMessageEvent;
import com.vimofthevine.underbudget.xml.budget.source.TemplateBudgetSource;

/**
 * A <code>BudgetPersistenceModel</code> is responsible for
 * all save/save-as/etc. operations on a session.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class BudgetPersistenceModel {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetPersistenceModel.class.getName());
	
	/**
	 * Session event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Application preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Budget source
	 */
	private BudgetSource source;
	
	/**
	 * Constructs a new budget persistence model.
	 * 
	 * @param bus    session event bus
	 * @param budget session budget
	 * @param source session budget source
	 * @param prefs  application preferences
	 */
	BudgetPersistenceModel(EventBus bus, Budget budget,
		BudgetSource source, UserPreferences prefs)
	{
		eventBus = bus;
		eventBus.register(this);
		
		preferences = prefs;
		
		this.budget = budget;
		this.source = source;
	}
	
	@Subscribe
	public void sourceSelected(BudgetSourceToSaveSelectedEvent event)
	{
		logger.log(Level.FINE, "Budget source selected");
		
		// Store budget source
		source = event.getSourceFactory().create(budget);
		
		// Perform save if source is valid (avoid infinite loop)
		if (source != null)
		{
			saveSession(new SaveSessionEvent());
			
			// Send out session info so this new session can be re-opened
			if (event.getSession() != null)
			{
				System.out.println("Sending out session-opened");
				eventBus.post(new SessionOpenedEvent(event.getSession()));
			}
		}
	}
	
	@Subscribe
	public void saveSession(SaveSessionEvent event)
	{
		// If source is the template, prompt for save source/location
		if (source == null || source instanceof TemplateBudgetSource)
		{
			logger.log(Level.FINE, "Write-only source, prompting for source location");
			saveSessionAs(new SaveSessionAsEvent());
		}
		else
		{
			try
			{
        		source.persist();
        		logger.log(Level.INFO, "Save successful");
        		eventBus.post(new SessionSavedEvent());
        		eventBus.post(new StatusMessageEvent("Budget saved to "
        			+ source.getDescription(), 5000));
			}
			catch (BudgetSourceException bse)
			{
				logger.log(Level.WARNING, "Unable to save budget", bse);
			}
		}
	}
	
	@Subscribe
	public void saveSessionAs(SaveSessionAsEvent event)
	{
		logger.log(Level.FINE, "Save-as received");
		eventBus.post(new SelectBudgetSourceToSaveEvent());
	}
	
	@Subscribe
	public void updateTemplate(UpdateTemplateEvent event)
	{
		logger.log(Level.FINE, "Updating the template");
		
		try
		{
    		BudgetSource template = new TemplateBudgetSource(
    			preferences.get("HOME", "/"), budget);
    		template.persist();
    		eventBus.post(new StatusMessageEvent("Template has been updated", 5000));
		}
		catch (BudgetSourceException bse)
		{
			logger.log(Level.WARNING, "Unable to update the template", bse);
		}
	}

}
