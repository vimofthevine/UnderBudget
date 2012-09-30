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

import com.vimofthevine.underbudget.swing.session.source.SourceSummary;

/**
 * Event generated when a session is opened so that it
 * can be added to the list of recently opened sessions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SessionOpenedEvent {
	
	/**
	 * Budget source summary
	 */
	private final SourceSummary summary;
	
	/**
	 * Constructs a new session-opened event.
	 * 
	 * @param summary budget source summary
	 */
	public SessionOpenedEvent(SourceSummary summary)
	{
		this.summary = summary;
	}
	
	/**
	 * Returns the recently opened session's
	 * budget source summary.
	 * 
	 * @return budget source summary
	 */
	public SourceSummary getSource()
	{
		return summary;
	}
	
	@Override
	public String toString()
	{
		return "Session opened event (" + summary + ")";
	}

}
