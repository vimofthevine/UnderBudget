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

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.events.RecentSessionsChangedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionOpenedEvent;
import com.vimofthevine.underbudget.swing.session.source.SourceSummary;
import com.vimofthevine.underbudget.swing.session.source.aes.AesEncryptedFileSummary;
import com.vimofthevine.underbudget.swing.session.source.xml.BudgetXmlFileSummary;

/**
 * Maintains the list of recently opened sessions, reading
 * and writing them to the user preferences.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RecentlyOpenedSessions {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(RecentlyOpenedSessions.class.getName());
	
	/**
	 * Maximum number of recent sessions to remember
	 */
	private static final int MAX = 5;
	
	/**
	 * Preference key prefix
	 */
	private static final String KEY = "RecentBudget";
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Recently opened sessions
	 */
	private final List<SourceSummary> sessions;

	/**
	 * Constructs a new list of recently opened sessions.
	 * 
	 * @param bus   event bus
	 * @param prefs user preferences
	 */
	public RecentlyOpenedSessions(EventBus bus, UserPreferences prefs)
	{
		eventBus = bus;
		eventBus.register(this);
		
		preferences = prefs;
		
		sessions = new ArrayList<SourceSummary>();
		
		for (int i=0; i<MAX; i++)
		{
			String type = preferences.get(KEY + i, "");
			
			try
			{
    			if (type.equals(AesEncryptedFileSummary.TYPE))
    			{
    				sessions.add(new AesEncryptedFileSummary(KEY+i, preferences));
    			}
    			else if (type.equals(BudgetXmlFileSummary.TYPE))
    			{
    				sessions.add(new BudgetXmlFileSummary(KEY+i, preferences));
    			}
			}
			catch (IllegalArgumentException iae)
			{
				logger.log(Level.WARNING, iae.getMessage());
			}
		}
		
		eventBus.post(new RecentSessionsChangedEvent(
			sessions.toArray(new SourceSummary[sessions.size()])));
	}
	
	@Subscribe
	public void sessionOpened(SessionOpenedEvent event)
	{
		SourceSummary session = event.getSource();
		
		if ( ! sessions.contains(session))
		{
			sessions.add(0, session);
			
			while (sessions.size() > MAX)
			{
				sessions.remove(MAX);
			}
			
			eventBus.post(new RecentSessionsChangedEvent(
				sessions.toArray(new SourceSummary[sessions.size()])));
		}
	}
	
	@Subscribe
	public void storeInPrefs(ApplicationShutdownEvent event)
	{
		for (int i=0; i<sessions.size(); i++)
		{
			sessions.get(i).persist(KEY + i, preferences);
		}
	}

}
