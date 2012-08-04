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

package com.vimofthevine.underbudget.swing.session.content;

import java.util.ArrayList;
import java.util.List;

/**
 * Displayed session content model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DisplayedContentModel {
	
	/**
	 * Registered displayed content listeners
	 */
	private final List<DisplayedContentListener> listeners;

	/**
	 * Currently displayed content
	 */
	private SessionContent currentContent;
	
	/**
	 * Constructs a new displayed content model.
	 */
	public DisplayedContentModel()
	{
		currentContent = SessionContent.ESTIMATE_PROGRESS;
		listeners = new ArrayList<DisplayedContentListener>();
	}
	
	/**
	 * Returns the currently displayed session content.
	 * 
	 * @return current session content
	 */
	public synchronized SessionContent getCurrentContent()
	{
		return currentContent;
	}
	
	/**
	 * Sets the currently displayed content, notifying all
	 * registered displayed content listeners.
	 * 
	 * @param content currently displayed session content
	 */
	public synchronized void setContent(SessionContent content)
	{
		currentContent = content;
		
		for (DisplayedContentListener listener : listeners)
		{
			listener.display(currentContent);
		}
	}
	
	/**
	 * Registers the given displayed content listener to
	 * receive notification of changes to the currently
	 * displayed session content.
	 * 
	 * @param listener displayed content listener to be registered
	 */
	public synchronized void addDisplayedContentListener(DisplayedContentListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters the given displayed content listener from
	 * receiving notification of changes to the currently
	 * displayed session content.
	 * 
	 * @param listener displayed content listener to be unregistered
	 */
	public synchronized void removeDisplayedContentListener(DisplayedContentListener listener)
	{
		listeners.remove(listener);
	}
	
}
