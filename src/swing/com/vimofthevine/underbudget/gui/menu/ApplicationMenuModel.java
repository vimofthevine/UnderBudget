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

package com.vimofthevine.underbudget.gui.menu;

import java.awt.event.ActionEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;

import com.google.common.eventbus.EventBus;

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
	 * Constructs a new application menu model.
	 * 
	 * @param bus   event bus
	 */
	public ApplicationMenuModel(EventBus bus)
	{
		eventBus = bus;
		actions = new HashMap<MenuAction,Action>();
		
		for (MenuAction action : MenuAction.values())
		{
			actions.put(action, createAction(action));
		}
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
	
	private Action createAction(final MenuAction action)
	{
		return new AbstractAction() {
			{
				putValue(NAME, action.getName());
				putValue(MNEMONIC_KEY, action.getMnemonic());
			}
			
			public void actionPerformed(ActionEvent event)
			{
				eventBus.post(action.getEvent());
			}
		};
	}
	
}
