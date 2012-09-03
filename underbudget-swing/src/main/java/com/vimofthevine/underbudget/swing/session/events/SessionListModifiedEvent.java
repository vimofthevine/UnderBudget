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

package com.vimofthevine.underbudget.swing.session.events;

import com.vimofthevine.underbudget.swing.session.Session;

/**
 * Event generated whenever a change occurs to the
 * list of open sessions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SessionListModifiedEvent {
	
	/**
	 * Current open session list
	 */
	private final Session[] sessions;
	
	/**
	 * Constructs a new session list modified event.
	 * 
	 * @param sessions current open session list
	 */
	public SessionListModifiedEvent(Session[] sessions)
	{
		this.sessions = sessions;
	}

	/**
	 * Returns the current list of open sessions.
	 * 
	 * @return current list of open sessions
	 */
	public Session[] getSessions()
	{
		return sessions;
	}
	
}
