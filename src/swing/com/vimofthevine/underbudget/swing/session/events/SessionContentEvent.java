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

import com.vimofthevine.underbudget.swing.session.content.SessionContent;

/**
 * An event generated when the user requests to
 * display a session-specific content view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SessionContentEvent implements SessionEvent {
	
	/**
	 * Requested view to be displayed
	 */
	private final SessionContent content;

	/**
	 * Constructs a new session content display
	 * event for the given content.
	 * 
	 * @param content requested session content
	 */
	public SessionContentEvent(SessionContent content)
	{
		this.content = content;
	}
	
	/**
	 * Returns the session content that has been
	 * requested to be displayed.
	 * 
	 * @return requested session content
	 */
	public final SessionContent getContent()
	{
		return content;
	}
	
}
