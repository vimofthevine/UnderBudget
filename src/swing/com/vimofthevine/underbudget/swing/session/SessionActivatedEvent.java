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

/**
 * An event that represents the activation
 * of a session.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SessionActivatedEvent {

	/**
	 * The session that has been activated
	 */
	private final Session session;
	
	/**
	 * Constructs a new session activation event.
	 * 
	 * @param session activated session
	 */
	public SessionActivatedEvent(Session session)
	{
		this.session = session;
	}
	
	/**
	 * Returns the session that has
	 * been activated.
	 * 
	 * @return activated session
	 */
	public Session getSession()
	{
		return session;
	}
	
	@Override
	public String toString()
	{
		return "Session " + session + " activated";
	}
	
}
