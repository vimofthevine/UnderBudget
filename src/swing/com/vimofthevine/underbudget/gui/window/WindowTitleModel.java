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

package com.vimofthevine.underbudget.gui.window;

import java.util.ArrayList;
import java.util.List;

import com.vimofthevine.underbudget.Application;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class WindowTitleModel {
	
	/**
	 * Registered window title listeners
	 */
	private final List<WindowTitleListener> listeners;
	
	/**
	 * Current window title
	 */
	private String title;
	
	public WindowTitleModel()
	{
		title = Application.TITLE;
		listeners = new ArrayList<WindowTitleListener>();
	}
	
	public synchronized String getTitle()
	{
		return title;
	}
	
	public synchronized void setTitle(String newTitle)
	{
		title = newTitle;
		
		for (WindowTitleListener listener : listeners)
		{
			listener.titleChanged(title);
		}
	}

	public synchronized void addWindowTitleListener(WindowTitleListener listener)
	{
		listeners.add(listener);
	}
	
	public synchronized void removeWindowTitleListener(WindowTitleListener listener)
	{
		listeners.remove(listener);
	}
	
}
