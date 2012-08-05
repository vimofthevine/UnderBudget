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

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionAsEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
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
	public void saveSession(SaveSessionEvent event)
	{
		source.persist();
		eventBus.post(new SessionSavedEvent());
	}
	
	@Subscribe
	public void saveSessionAs(SaveSessionAsEvent event)
	{
		// TODO
	}
	
	@Subscribe
	public void updateTemplate(UpdateTemplateEvent event)
	{
		// TODO
	}

}
