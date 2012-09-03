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

import javax.swing.DefaultComboBoxModel;

/**
 * Custom combo box model with support for registering
 * listeners to be notified of changes to the selected item.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ObservableComboBoxModel extends DefaultComboBoxModel {
	
	/**
	 * Item selection change listeners
	 */
	private final Collection<ItemSelectionListener> listeners;
	
	/**
	 * Constructs a new observable combo box model.
	 * 
	 * @param items items available for selection
	 */
	public ObservableComboBoxModel(Object[] items)
	{
		super(items);
		
		listeners = Collections.synchronizedList(
			new ArrayList<ItemSelectionListener>());
	}
	
	/**
	 * Registers a listener to be notified of changes
	 * to the item selection.
	 * 
	 * @param listener selection change listener to be registered
	 */
	public void addItemSelectionListener(ItemSelectionListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from being notified of changes
	 * to the item selection.
	 * 
	 * @param listener selection change listener to be unregistered
	 */
	public void removeItemSelectionListener(ItemSelectionListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Sets the currently selected item, optionally
	 * notifying registered listeners of the selection change.
	 * 
	 * @param item   newly selected item
	 * @param notify whether to notify listeners
	 */
	public void setSelectedItem(final Object item,
		boolean notify)
	{
		super.setSelectedItem(item);
		
		if (notify)
		{
			synchronized(listeners)
			{
				for (ItemSelectionListener listener : listeners)
				{
					listener.itemSelected(item);
				}
			}
		}
	}

	@Override
	public void setSelectedItem(Object item)
	{
		setSelectedItem(item, true);
	}
	
}
