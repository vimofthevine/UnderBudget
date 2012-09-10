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
import java.util.Date;

import org.jdesktop.swingx.calendar.DefaultDateSelectionModel;

/**
 * Custom date selection model with support for registering
 * listeners to be notified of changes to the selected date.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ObservableDateSelectionModel extends DefaultDateSelectionModel {
	
	/**
	 * Date selection change listeners
	 */
	private final Collection<SelectedDateListener> listeners;

	public ObservableDateSelectionModel()
	{
		super();
		
		listeners = Collections.synchronizedList(
			new ArrayList<SelectedDateListener>());
	}
	
	/**
	 * Registers a listener to be notified of changes
	 * to the date selection.
	 * 
	 * @param listener selection change listener to be registered
	 */
	public void addSelectedDateListener(SelectedDateListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from being notified of changes
	 * to the date selection.
	 * 
	 * @param listener selection change listener to be unregistered
	 */
	public void removeSelectedDateListener(SelectedDateListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Clears the selection, optionally notifying
	 * registered listeners of the selection change.
	 * 
	 * @param notify whether to notify listeners
	 */
	public void clearSelection(boolean notify)
	{
		super.clearSelection();
		
		if (notify)
		{
			update();
		}
	}
	
	/**
	 * Updates the selected date interval, optionally
	 * notifying registered listeners of the selection change.
	 * 
	 * @param start  selected interval start date
	 * @param end    selected interval end date
	 * @param notify whether to notify listeners
	 */
	public void setSelectionInterval(Date start, Date end, boolean notify)
	{
		super.setSelectionInterval(start, end);
		
		if (notify)
		{
			update();
		}
	}
	
	/**
	 * Updates the selected date interval, optionally
	 * notifying registered listeners of the selection change.
	 * 
	 * @param start  selected interval start date
	 * @param end    selected interval end date
	 * @param notify whether to notify listeners
	 */
	public void addSelectionInterval(Date start, Date end, boolean notify)
	{
		super.addSelectionInterval(start, end);
		
		if (notify)
		{
			update();
		}
	}
	
	@Override
	public void clearSelection()
	{
		clearSelection(true);
	}
	
	@Override
	public void setSelectionInterval(Date start, Date end)
	{
		setSelectionInterval(start, end, true);
	}
	
	@Override
	public void addSelectionInterval(Date start, Date end)
	{
		addSelectionInterval(start, end, true);
	}
	
	/**
	 * Notifies all registered listeners of a date selection change.
	 */
	private void update()
	{
		synchronized (listeners)
		{
			for (SelectedDateListener listener : listeners)
			{
				listener.dateSelectionChanged();
			}
		}
	}
	
}
