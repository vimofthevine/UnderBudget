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

package com.vimofthevine.underbudget.swing.menu;

import java.awt.event.ActionEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.session.Session;
import com.vimofthevine.underbudget.swing.session.events.ActivateSessionEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionActivatedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionListModifiedEvent;
import com.vimofthevine.underbudget.swing.session.events.SessionStateEvent;

/**
 * A presentation model for the application
 * menu and toolbar.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ApplicationMenuModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Actions
	 */
	private final Map<MenuAction, Action> actions;
	
	/**
	 * List of open sessions
	 */
	private Session[] sessions;
	
	/**
	 * Constructs a new application menu model.
	 * 
	 * @param bus   event bus
	 */
	public ApplicationMenuModel(EventBus bus)
	{
		eventBus = bus;
		eventBus.register(this);
		
		actions = new HashMap<MenuAction,Action>();
		sessions = new Session[0];
		
		for (MenuAction action : MenuAction.values())
		{
			actions.put(action, action.getAction(eventBus));
		}
		
		// Disable actions that are not initially available
		actions.get(MenuAction.SAVE_SESSION).setEnabled(false);
		actions.get(MenuAction.SAVE_SESSION_AS).setEnabled(false);
		actions.get(MenuAction.SAVE_SESSION_AS_TEMPLATE).setEnabled(false);
		actions.get(MenuAction.CLOSE_SESSION).setEnabled(false);
		actions.get(MenuAction.EXPORT_RESULTS).setEnabled(false);
		actions.get(MenuAction.BUDGET_DISPLAY).setEnabled(false);
		actions.get(MenuAction.ASSIGNMENT_RULES).setEnabled(false);
		actions.get(MenuAction.EDIT_ESTIMATES).setEnabled(false);
		actions.get(MenuAction.ESTIMATE_PROGRESS).setEnabled(false);
		actions.get(MenuAction.BALANCE_IMPACT).setEnabled(false);
		actions.get(MenuAction.IMPORTED_TRANSACTIONS).setEnabled(false);
		actions.get(MenuAction.ANALYSIS_SUMMARY).setEnabled(false);
		actions.get(MenuAction.IMPORT_TRANSACTIONS).setEnabled(false);
		actions.get(MenuAction.IMPORT_TRANSACTIONS_FROM).setEnabled(false);
		actions.get(MenuAction.ASSIGN_TRANSACTIONS).setEnabled(false);
		actions.get(MenuAction.CALCULATE_BALANCES).setEnabled(false);
	}
	
	/**
	 * Returns the action model for the
	 * specified menu action.
	 * 
	 * @param action menu action
	 * @return menu action model
	 */
	Action getAction(MenuAction action)
	{
		return actions.get(action);
	}
	
	/**
	 * Returns the action models for all
	 * open sessions.
	 * 
	 * @return open session menu actions
	 */
	Action[] getOpenSessionActions()
	{
		Action[] actions = new Action[sessions.length];
		
		for (int i=0; i<sessions.length; i++)
		{
			final int num = i;
			final Session session = sessions[i];
			actions[i] = new AbstractAction() {
				{
					putValue(NAME, (num+1) + " " + session.getName() +
						(session.isDirty() ? "*" : ""));
					putValue(SELECTED_KEY, session.isActive());
				}
				
				public void actionPerformed(ActionEvent event)
				{
					eventBus.post(new ActivateSessionEvent(session));
				}
			};
		}
		
		return actions;
	}
	
	@Subscribe
	public void sessionActivated(final SessionActivatedEvent event)
	{
		// If an active session exists
		if (event.getSession() != null)
		{
    		// Enable all session-specific events
    		SwingUtilities.invokeLater(new Runnable() {
    			public void run()
    			{
    				actions.get(MenuAction.SAVE_SESSION).setEnabled(event.getSession().isDirty());
            		actions.get(MenuAction.SAVE_SESSION_AS).setEnabled(true);
            		actions.get(MenuAction.SAVE_SESSION_AS_TEMPLATE).setEnabled(true);
            		actions.get(MenuAction.CLOSE_SESSION).setEnabled(true);
            		actions.get(MenuAction.EXPORT_RESULTS).setEnabled(true);
            		actions.get(MenuAction.BUDGET_DISPLAY).setEnabled(true);
            		actions.get(MenuAction.ASSIGNMENT_RULES).setEnabled(true);
            		actions.get(MenuAction.EDIT_ESTIMATES).setEnabled(true);
            		actions.get(MenuAction.ESTIMATE_PROGRESS).setEnabled(true);
            		actions.get(MenuAction.BALANCE_IMPACT).setEnabled(true);
            		actions.get(MenuAction.IMPORTED_TRANSACTIONS).setEnabled(true);
            		actions.get(MenuAction.ANALYSIS_SUMMARY).setEnabled(true);
            		actions.get(MenuAction.IMPORT_TRANSACTIONS).setEnabled(true);
            		actions.get(MenuAction.IMPORT_TRANSACTIONS_FROM).setEnabled(true);
            		actions.get(MenuAction.ASSIGN_TRANSACTIONS).setEnabled(true);
            		actions.get(MenuAction.CALCULATE_BALANCES).setEnabled(true);
    			}
    		});
		}
		else
		{
			// Disable all session-specific actions
    		SwingUtilities.invokeLater(new Runnable() {
    			public void run()
    			{
            		actions.get(MenuAction.SAVE_SESSION).setEnabled(false);
            		actions.get(MenuAction.SAVE_SESSION_AS).setEnabled(false);
            		actions.get(MenuAction.SAVE_SESSION_AS_TEMPLATE).setEnabled(false);
            		actions.get(MenuAction.CLOSE_SESSION).setEnabled(false);
            		actions.get(MenuAction.EXPORT_RESULTS).setEnabled(false);
            		actions.get(MenuAction.BUDGET_DISPLAY).setEnabled(false);
            		actions.get(MenuAction.ASSIGNMENT_RULES).setEnabled(false);
            		actions.get(MenuAction.EDIT_ESTIMATES).setEnabled(false);
            		actions.get(MenuAction.ESTIMATE_PROGRESS).setEnabled(false);
            		actions.get(MenuAction.BALANCE_IMPACT).setEnabled(false);
            		actions.get(MenuAction.IMPORTED_TRANSACTIONS).setEnabled(false);
            		actions.get(MenuAction.ANALYSIS_SUMMARY).setEnabled(false);
            		actions.get(MenuAction.IMPORT_TRANSACTIONS).setEnabled(false);
            		actions.get(MenuAction.IMPORT_TRANSACTIONS_FROM).setEnabled(false);
            		actions.get(MenuAction.ASSIGN_TRANSACTIONS).setEnabled(false);
            		actions.get(MenuAction.CALCULATE_BALANCES).setEnabled(false);
    			}
    		});
		}
	}
	
	@Subscribe
	public void sessionStateUpdate(final SessionStateEvent event)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				actions.get(MenuAction.SAVE_SESSION).setEnabled(event.isDirty());
			}
		});
	}
	
	@Subscribe
	public void sessionListChanged(SessionListModifiedEvent event)
	{
		sessions = event.getSessions();
	}
	
}
