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

package com.vimofthevine.underbudget.swing.tutorial;

import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

/**
 * Tutorial content model
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class TutorialContentModel {
	
	/**
	 * Registered tutorial content listeners
	 */
	private final Collection<TutorialContentListener> listeners;
	
	/**
	 * Current content URL
	 */
	private URL currentUrl;
	
	/**
	 * Constructs a new tutorial content model instance.
	 */
	TutorialContentModel()
	{
		listeners = Collections.synchronizedCollection(
			new ArrayList<TutorialContentListener>());
	}
	
	/**
	 * Registers a content listener to be notified of
	 * changes to the tutorial content.
	 * 
	 * @param listener tutorial content listeners
	 */
	void addContentListener(TutorialContentListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Updates the tutorial content URL.
	 * 
	 * @param url new tutorial content URL
	 */
	void setContent(final URL url)
	{
		currentUrl = url;
		
		synchronized(listeners)
		{
    		for (TutorialContentListener listener : listeners)
    		{
    			listener.contentChanged(url);
    		}
		}
	}
	
	/**
	 * Returns the current content URL.
	 * 
	 * @return current content URL
	 */
	URL getContent()
	{
		return currentUrl;
	}

}
