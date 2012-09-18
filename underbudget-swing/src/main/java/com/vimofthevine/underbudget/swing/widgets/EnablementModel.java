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

/**
 * Model for encapsulating enabled/disabled state.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EnablementModel {
	
	/**
	 * Registered enablement change listeners
	 */
	private final Collection<EnablementListener> listeners;
	
	/**
	 * Constructs a new enablement model.
	 */
	public EnablementModel()
	{
		listeners = Collections.synchronizedList(
			new ArrayList<EnablementListener>());
	}
	
	/**
	 * Registers a listener to be notified of changes
	 * to the enabled state.
	 * 
	 * @param listener enabled state change listener to be registered
	 */
	public void addListener(EnablementListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from being notified of changes
	 * to the enabled state.
	 * 
	 * @param listener enabled state change listener to be unregistered
	 */
	public void removeListener(EnablementListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Updates the enabled state.
	 * 
	 * @param enabled <code>true</code> if enabled,
	 *                else <code>false</code>
	 */
	public void setEnabled(boolean enabled)
	{
		synchronized (listeners)
		{
			for (EnablementListener listener : listeners)
			{
				listener.enable(enabled);
			}
		}
	}

}
