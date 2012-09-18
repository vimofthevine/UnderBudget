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

/**
 * Event generated to indicate the current
 * state of the current session.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SessionStateEvent {
	
	/**
	 * Session name
	 */
	private final String name;
	
	/**
	 * Whether the session is dirty
	 */
	private final boolean isDirty;
	
	/**
	 * Constructs a new session state event.
	 * 
	 * @param name    session name
	 * @param isDirty whether session has unsaved changes
	 */
	public SessionStateEvent(String name, boolean isDirty)
	{
		this.name = name;
		this.isDirty = isDirty;
	}

	/**
	 * Returns the session name.
	 * 
	 * @return session name
	 */
	public String getSessionName()
	{
		return name;
	}
	
	/**
	 * Returns whether the session has
	 * unsaved changes.
	 * 
	 * @return <code>true</code> if the session
	 *         has unsaved changes, else <code>false</code>
	 */
	public boolean isDirty()
	{
		return isDirty;
	}
	
}
