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

package com.vimofthevine.underbudget.swing.status;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

/**
 * Progress model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ProgressModel {

	/**
	 * Progress action listeners
	 */
	private final Collection<ProgressListener> listeners;
	
	/**
	 * Constructs a new progress model.
	 */
	ProgressModel()
	{
		listeners = Collections.synchronizedList(
			new ArrayList<ProgressListener>());
	}
	
	/**
	 * Registers a new progress listener to be
	 * notified of progress actions.
	 * 
	 * @param listener progress listeners to be registered
	 */
	void addListener(ProgressListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a progress listeners from receiving
	 * progress action notifications.
	 * 
	 * @param listener progress listener to be unregistered
	 */
	void removeListener(ProgressListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Notifies all listeners that the progress is
	 * indeterminate or nondeterminate.
	 * 
	 * @param indeterminate <code>true</code> if indeterminate
	 */
	void setIndeterminate(final boolean indeterminate)
	{
		synchronized(listeners)
		{
			for (ProgressListener listener : listeners)
			{
				listener.setIndeterminate(indeterminate);
			}
		}
	}
	
	/**
	 * Notifies all listeners of the new progress percent
	 * value.
	 * 
	 * @param value progress percent value
	 */
	void setValue(final int value)
	{
		synchronized(listeners)
		{
			for (ProgressListener listener : listeners)
			{
				listener.setValue(value);
			}
		}
	}

}
