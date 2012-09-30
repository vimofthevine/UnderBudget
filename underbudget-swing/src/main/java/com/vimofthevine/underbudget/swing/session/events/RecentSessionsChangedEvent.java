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

import com.vimofthevine.underbudget.swing.session.recent.RecentSession;

/**
 * Event generated when the list of recently opened sessions
 * has changed, so the application menu can be updated.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RecentSessionsChangedEvent {
	
	/**
	 * List of recently opened sessions
	 */
	private final RecentSession[] sessions;

	/**
	 * Constructs a new recent sessions changed event.
	 * 
	 * @param sessions new list of recently opened sessions
	 */
	public RecentSessionsChangedEvent(RecentSession[] sessions)
	{
		this.sessions = sessions;
	}
	
	/**
	 * Returns the list of recently opened sessions.
	 * 
	 * @return recently opened sessions
	 */
	public RecentSession[] getSessions()
	{
		return sessions;
	}
	
	@Override
	public String toString()
	{
		return "Recently opened sessions event";
	}

}
