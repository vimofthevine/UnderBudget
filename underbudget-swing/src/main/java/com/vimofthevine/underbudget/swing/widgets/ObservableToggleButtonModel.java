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

package com.vimofthevine.underbudget.swing.widgets;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

import javax.swing.JToggleButton.ToggleButtonModel;

/**
 * Custom toggle button model with support for registering
 * listeners to be notified of changes to the toggle selection.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ObservableToggleButtonModel extends ToggleButtonModel {
	
	/**
	 * Toggle selection listeners
	 */
	private final Collection<SelectionToggleListener> listeners;
	
	/**
	 * Constructs a new observable toggle button model.
	 */
	public ObservableToggleButtonModel()
	{
		super();
		
		listeners = Collections.synchronizedList(
			new ArrayList<SelectionToggleListener>());
	}
	
	/**
	 * Registers a listener to be notified of changes
	 * to the toggle selection.
	 * 
	 * @param listener selection toggle listener to be registered
	 */
	public void addSelectionToggleListener(SelectionToggleListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from being notified of changes
	 * to the toggle selection.
	 * 
	 * @param listener selection toggle listener to be unregistered
	 */
	public void removeSelectionToggleListener(SelectionToggleListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Sets the current toggle selection, optionally
	 * notifying registered listeners of the selection
	 * change.
	 * 
	 * @param selected toggle selection
	 * @param notify   whether to notify listeners
	 */
	public void setSelected(boolean selected, boolean notify)
	{
		super.setSelected(selected);
		
		if (notify)
		{
			synchronized(listeners)
			{
				for (SelectionToggleListener listener : listeners)
				{
					listener.selectionToggled(selected);
				}
			}
		}
	}

	@Override
	public void setSelected(boolean selected)
	{
		setSelected(selected, true);
	}
	
}
