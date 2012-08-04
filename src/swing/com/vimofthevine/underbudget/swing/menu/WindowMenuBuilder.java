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
import java.awt.event.ActionListener;

import javax.swing.Action;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.MenuSelectionManager;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

/**
 * Menu listener to build the window menu to
 * include 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class WindowMenuBuilder implements MenuListener {
	
	/**
	 * Menu model
	 */
	private final ApplicationMenuModel model;
	
	/**
	 * Constructs a new window menu builder.
	 * 
	 * @param model application menu model
	 */
	WindowMenuBuilder(ApplicationMenuModel model)
	{
		this.model = model;
	}

	@Override
	public void menuCanceled(MenuEvent event)
	{
		// Do nothing
	}

	@Override
	public void menuDeselected(MenuEvent event)
	{
		// Do nothing
	}

	/**
	 * Rebuilds the window menu to include
	 * all open sessions.
	 */
	@Override
	public void menuSelected(MenuEvent event)
	{
		final JMenu menu = (JMenu) event.getSource();
		menu.removeAll();
		
		int i = 0;
		
		for (Action session : model.getOpenSessionActions())
		{
			JRadioButton item = new JRadioButton(session);
			item.addActionListener(new ActionListener() {

				@Override
                public void actionPerformed(ActionEvent e)
                {
					MenuSelectionManager.defaultManager().clearSelectedPath();
                }
				
			});
			menu.add(item);
			i++;
		}
		
		if (i > 0)
		{
			menu.add(new JSeparator());
		}
		
		menu.add(new JMenuItem(model.getAction(MenuAction.NEW_WINDOW)));
	}

}
