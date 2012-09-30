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

package com.vimofthevine.underbudget.swing.session.source;

import java.awt.Frame;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceFactory;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToOpenSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToSaveSelectedEvent;
import com.vimofthevine.underbudget.swing.session.recent.RecentSession;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SelectSourceForAction {
	
	/**
	 * Whether the action is an open or save action
	 */
	private final boolean isOpen;
	
	/**
	 * Source type to be selected
	 */
	private final SourceType sourceType;
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Parent window
	 */
	private final Frame window;
	
	/**
	 * @param isOpenAction
	 * @param type
	 * @param bus
	 * @param prefs
	 * @param parent
	 */
	public SelectSourceForAction(boolean isOpenAction, SourceType type,
		EventBus bus, UserPreferences prefs, Frame parent)
	{
		isOpen = isOpenAction;
		sourceType = type;
		eventBus = bus;
		preferences = prefs;
		window = parent;
	}
	
	public SourceType getSourceType()
	{
		return sourceType;
	}
	
	public boolean isOpenEvent()
	{
		return isOpen;
	}
	
	public Frame getParentWindow()
	{
		return window;
	}
	
	public UserPreferences getPreferences()
	{
		return preferences;
	}
	
	public void select(BudgetSourceFactory factory,
		RecentSession session)
	{
		if (isOpen)
		{
			eventBus.post(new BudgetSourceToOpenSelectedEvent(factory, session));
		}
		else
		{
			eventBus.post(new BudgetSourceToSaveSelectedEvent(factory, session));
		}
	}

}
