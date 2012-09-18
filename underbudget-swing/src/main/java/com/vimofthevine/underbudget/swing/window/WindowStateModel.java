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

package com.vimofthevine.underbudget.swing.window;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

/**
 * Window state model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class WindowStateModel {
	
	/**
	 * Registered state listeners
	 */
	private final Collection<WindowStateListener> listeners;
	
	/**
	 * Constructs a new window state model instance.
	 */
	public WindowStateModel()
	{
		listeners = Collections.synchronizedCollection(
			new ArrayList<WindowStateListener>());
	}
	
	/**
	 * Registers a listener to be notified of intended
	 * changes to a window's state.
	 * 
	 * @param listener window state listener
	 */
	public void addStateListener(WindowStateListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Notifies window state listeners of the intention
	 * to close the window.
	 */
	public void close()
	{
		synchronized(listeners)
		{
			for (WindowStateListener listener : listeners)
			{
				listener.closeWindow();
			}
		}
	}

}
