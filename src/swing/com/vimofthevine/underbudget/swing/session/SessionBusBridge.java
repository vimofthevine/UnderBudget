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
import com.vimofthevine.underbudget.swing.session.events.SessionEvent;

/**
 * Bridge between the global application event bus
 * and the active session event bus.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class SessionBusBridge {
	
	/**
	 * The active session
	 */
	private Session activeSession;
	
	/**
	 * Constructs a session bus bridge.
	 * 
	 * @param bus global event bus
	 */
	public SessionBusBridge(EventBus bus)
	{
		bus.register(this);
	}
	
	/**
	 * Sets the active session.
	 * 
	 * @param session active session
	 */
	void setActiveSession(Session session)
	{
		activeSession = session;
	}
	
	@Subscribe
	public void eventPosted(SessionEvent event)
	{
		if (activeSession != null)
		{
			activeSession.post(event);
		}
	}

}
