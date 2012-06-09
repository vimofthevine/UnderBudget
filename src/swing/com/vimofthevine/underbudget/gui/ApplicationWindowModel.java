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

package com.vimofthevine.underbudget.gui;

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import com.google.common.eventbus.EventBus;

/**
 * Presentation model for the application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ApplicationWindowModel {
	
	/**
	 * Window height preferences key
	 */
	private static final String HEIGHT_KEY = "Window.Height";
	
	/**
	 * Window width preferences key
	 */
	private static final String WIDTH_KEY = "Window.Width";
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Flag to track whether the window's size
	 * has been initialized yet (to avoid saving
	 * the uninitialized size)
	 */
	private boolean sizeHasBeenSet = true;
	
	/**
	 * Constructs a new application window
	 * presentation model
	 * 
	 * @param bus   event bus
	 * @param prefs user preferences
	 */
	public ApplicationWindowModel(EventBus bus, UserPreferences prefs)
	{
		eventBus = bus;
		preferences = prefs;
	}
	
	public ComponentListener getComponentListener()
	{
		return new ComponentAdapter()
		{
			/**
			 * Stores the new window dimensions as user preferences.
			 */
			@Override
            public void componentResized(ComponentEvent event)
            {
				if (sizeHasBeenSet)
				{
    				int width = event.getComponent().getWidth();
    				int height = event.getComponent().getHeight();
    				preferences.set(WIDTH_KEY, String.valueOf(width));
    				preferences.set(HEIGHT_KEY, String.valueOf(height));
				}
            }
		};
	}
	
	public WindowListener getWindowListener()
	{
		return new WindowAdapter()
		{
			/**
			 * Generates a shutdown event as a result of the
			 * user hitting the close button on the window.
			 */
			@Override
            public void windowClosing(WindowEvent event)
            {
				eventBus.post(new ApplicationShutdownEvent());
            }
		};
	}
	
	public int getWindowWidth()
	{
		return Integer.parseInt(preferences.get(WIDTH_KEY, "700"));
	}
	
	public int getWindowHeight()
	{
		return Integer.parseInt(preferences.get(HEIGHT_KEY, "650"));
	}

}
