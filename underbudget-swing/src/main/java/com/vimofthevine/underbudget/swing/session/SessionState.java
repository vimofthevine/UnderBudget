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
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.swing.history.ModificationEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionSavedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionStateEvent;

/**
 * A <code>SessionState</code> keeps track of the
 * state of unsaved modifications made to a session's
 * budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class SessionState {
	
	/**
	 * Application event bus
	 */
	private final EventBus globalBus;
	
	/**
	 * Session budget
	 */
	private final Budget budget;
	
	/**
	 * Whether the session is "dirty"
	 */
	private boolean isDirty;
	
	/**
	 * Constructs a new session state instance.
	 * 
	 * @param globalBus  application event bus
	 * @param sessionBus session event bus
	 */
	SessionState(EventBus globalBus, EventBus sessionBus,
		Budget budget)
	{
		this.globalBus = globalBus;
		this.budget = budget;
		isDirty = false;
		
		sessionBus.register(this);
	}
	
	/**
	 * Checks if the session has any unsaved modifications
	 * (i.e., is dirty).
	 * 
	 * @return <code>true</code> if the session is dirty,
	 *         else <code>false</code>
	 */
	boolean isDirty()
	{
		return isDirty;
	}
	
	/**
	 * Marks the session as dirty (i.e., a modification was made).
	 * 
	 * @param event modification event
	 */
	@Subscribe
	public void modificationMade(ModificationEvent event)
	{
		isDirty = true;
		fireUpdate();
	}
	
	/**
	 * Marks the session as clean (all changes have been saved).
	 * 
	 * @param event session saved event
	 */
	@Subscribe
	public void sessionSaved(SessionSavedEvent event)
	{
		isDirty = false;
		fireUpdate();
	}
	
	private void fireUpdate()
	{
		globalBus.post(new SessionStateEvent(
			budget.getDefinition().getName(), isDirty));
	}

}
