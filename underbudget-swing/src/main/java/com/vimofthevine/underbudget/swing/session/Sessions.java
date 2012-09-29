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

import java.awt.Frame;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JOptionPane;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.swing.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.events.ActivateSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToOpenSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.CloseSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.CreateSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.OpenSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SaveSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToOpenEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionActivatedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionListModifiedEvent;
import com.vimofthevine.underbudget.swing.session.wizard.BudgetSourceSelectionWizard;
import com.vimofthevine.underbudget.swing.widgets.ErrorPopup;
import com.vimofthevine.underbudget.xml.budget.source.TemplateBudgetSource;

/**
 * A <code>Sessions</code> instance is reponsible for creating,
 * activating, and closing sessions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Sessions {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(Sessions.class.getName());
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Active session bus bridge
	 */
	private final SessionBusBridge busBridge;
	
	/**
	 * Open sessions
	 */
	private final List<Session> sessions;
	
	/**
	 * Active session
	 */
	private Session activeSession;
	
	/**
	 * Constructs a session manager.
	 * 
	 * @param window application window
	 * @param bus    application event bus
	 * @param prefs  user preferences
	 */
	public Sessions(Frame window, EventBus bus,
		UserPreferences prefs)
	{
		this.window = window;
		preferences = prefs;
		eventBus = bus;
		eventBus.register(this);
		
		busBridge = new SessionBusBridge(eventBus);
		
		new BudgetSourceSelectionWizard(bus, window, null, prefs);
		sessions = new ArrayList<Session>();
	}
	
	/**
	 * Updates the active session, marking the previous
	 * active session as inactive and the new session
	 * as active.
	 * 
	 * @param session session to be activated
	 */
	private void setActive(Session session)
	{
		if (activeSession != null)
		{
			activeSession.setActive(false);
		}
		
		activeSession = session;
		busBridge.setActiveSession(session);
		
		if (activeSession != null)
		{
			activeSession.setActive(true);
		}
		
		eventBus.post(new SessionActivatedEvent(activeSession));
	}

	@Subscribe
	public void createSession(CreateSessionEvent event)
	{
		logger.log(Level.INFO, "Creating new session");
		createSession(new TemplateBudgetSource(preferences.get("HOME", "/tmp")));
	}
	
	/**
	 * Launches the budget source selection wizard,
	 * allowing the user to select a budget to be opened.
	 * 
	 * @param event
	 */
	@Subscribe
	public void openSession(OpenSessionEvent event)
	{
		logger.log(Level.INFO, "Opening new session");
		eventBus.post(new SelectBudgetSourceToOpenEvent());
		
	}
	
	@Subscribe
	public void sourceSelected(BudgetSourceToOpenSelectedEvent event)
	{
		logger.log(Level.FINE, "Budget source selected");
		createSession(event.getSource());
	}
	
	private void createSession(BudgetSource source)
	{
		try
		{
			Session newSession = new Session(window,
				eventBus, preferences, source);
		
    		sessions.add(newSession);
    		setActive(newSession);
    		
    		eventBus.post(new SessionListModifiedEvent(
    			sessions.toArray(new Session[sessions.size()])));
		}
		catch (BudgetSourceException bse)
		{
			logger.log(Level.WARNING, "Unable to create session", bse);
			new ErrorPopup(bse, window);
		}
	}
	
	@Subscribe
	public void activateSession(ActivateSessionEvent event)
	{
		setActive(event.getSession());
	}
	
	@Subscribe
	public void closeSession(CloseSessionEvent event)
	{
		if (activeSession != null)
		{
			if (activeSession.isDirty())
			{
				int result = JOptionPane.showConfirmDialog(window, activeSession.getName() +
					" has unsaved changes. Do you wish to save these changes?",
					"Unsaved changes", JOptionPane.YES_NO_CANCEL_OPTION);
				
				if (result == JOptionPane.YES_OPTION)
				{
					activeSession.post(new SaveSessionEvent());
				}
				else if (result == JOptionPane.CANCEL_OPTION)
				{
					return;
				}
			}
			
			sessions.remove(activeSession);
			
			setActive((sessions.size() > 0)
				? sessions.get(0) : null);
			
    		eventBus.post(new SessionListModifiedEvent(
    			sessions.toArray(new Session[sessions.size()])));
		}
	}
	
	@Subscribe
	public void shutdown(ApplicationShutdownEvent event)
	{
		for (Session session : sessions)
		{
			if (session.isDirty())
			{
				int result = JOptionPane.showConfirmDialog(window, activeSession.getName() +
					" has unsaved changes. Do you wish to save these changes?",
					"Unsaved changes", JOptionPane.YES_NO_OPTION);
				
				if (result == JOptionPane.YES_OPTION)
				{
					activeSession.post(new SaveSessionEvent());
				}
			}
		}
	}
	
}
