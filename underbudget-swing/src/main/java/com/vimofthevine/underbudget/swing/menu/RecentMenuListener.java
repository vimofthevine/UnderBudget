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

import javax.swing.Action;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

/**
 * Menu listener to build the open-recent menu.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class RecentMenuListener implements MenuListener {
	
	/**
	 * Menu model
	 */
	private final ApplicationMenuModel model;
	
	/**
	 * Constructs a new recent session menu builder.
	 * 
	 * @param model application menu model
	 */
	RecentMenuListener(ApplicationMenuModel model)
	{
		this.model = model;
	}

	@Override
	public void menuCanceled(MenuEvent arg0)
	{
		// Do nothing
	}

	@Override
	public void menuDeselected(MenuEvent arg0)
	{
		// Do nothing
	}

	@Override
	public void menuSelected(MenuEvent event)
	{
		final JMenu menu = (JMenu) event.getSource();
		menu.removeAll();
		
		for (Action session : model.getRecentSessionAction())
		{
			menu.add(new JMenuItem(session));
		}
	}

}
