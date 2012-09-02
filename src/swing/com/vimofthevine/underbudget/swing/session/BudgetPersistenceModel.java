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
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.core.budget.source.TemplateBudgetSource;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToSaveSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionAsEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToOpenEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionSavedEvent;
import com.vimofthevine.underbudget.swing.session.events.UpdateTemplateEvent;

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
	 * Budget source
	 */
	private BudgetSource source;
	
	/**
	 * Constructs a new budget persistence model.
	 * 
	 * @param bus    session event bus
	 * @param source session budget source
	 */
	BudgetPersistenceModel(EventBus bus,
		BudgetSource source)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.source = source;
	}
	
	@Subscribe
	public void sourceSelected(BudgetSourceToSaveSelectedEvent event)
	{
		logger.log(Level.INFO, "Budget source selected");
		
		// Store budget source
		source = event.getSource();
		
		// Perform save if source is valid (avoid infinite loop)
		if (source != null)
		{
			saveSession(new SaveSessionEvent());
		}
	}
	
	@Subscribe
	public void saveSession(SaveSessionEvent event)
	{
		// If source is the template, prompt for save source/location
		if (source == null || source instanceof TemplateBudgetSource)
		{
			logger.log(Level.INFO, "Write-only source, prompting for source location");
			saveSessionAs(new SaveSessionAsEvent());
		}
		else
		{
			try
			{
        		source.persist();
        		eventBus.post(new SessionSavedEvent());
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
		eventBus.post(new SelectBudgetSourceToOpenEvent());
	}
	
	@Subscribe
	public void updateTemplate(UpdateTemplateEvent event)
	{
		logger.log(Level.INFO, "Updating the template");
		
		try
		{
    		BudgetSource template = new TemplateBudgetSource(
    			source.getBudget());
    		template.persist();
		}
		catch (BudgetSourceException bse)
		{
			logger.log(Level.WARNING, "Unable to update the template", bse);
		}
	}

}
