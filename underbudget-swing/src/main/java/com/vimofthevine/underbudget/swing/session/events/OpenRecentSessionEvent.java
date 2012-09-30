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
 * Event generated to open a recently opened session.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OpenRecentSessionEvent {
	
	private final RecentSession session;
	
	/**
	 * Constructs a new open recent session event.
	 * 
	 * @param session recent session to open
	 */
	public OpenRecentSessionEvent(RecentSession session)
	{
		this.session = session;
	}
	
	/**
	 * Returns the recent session to be re-opened.
	 * 
	 * @return recent session to open
	 */
	public RecentSession getSession()
	{
		return session;
	}
	
	@Override
	public String toString()
	{
		return "Open recent session event (" + session + ")";
	}

}
