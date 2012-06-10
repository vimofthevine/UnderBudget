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

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.gui.ApplicationShutdownEvent;
import com.vimofthevine.underbudget.gui.UserPreferences;
import com.vimofthevine.underbudget.gui.session.SessionActivatedEvent;
import com.vimofthevine.underbudget.gui.session.SessionStateEvent;

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
	 * Window title model
	 */
	private final WindowTitleModel titleModel;
	
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
		
		titleModel = new WindowTitleModel();
	}
	
	/**
	 * Creates a component listener to record resizes
	 * to the application window so the window size
	 * can be re-applied the next time the application
	 * is executed.
	 * 
	 * @return application window resize listener
	 */
	ComponentListener getComponentListener()
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
	
	/**
	 * Creates a window listener to respond to the
	 * user event to close the application window.
	 * 
	 * @return application window close listener
	 */
	WindowListener getWindowListener()
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
	
	/**
	 * Returns the model for the application window title.
	 * 
	 * @return application window title model
	 */
	WindowTitleModel getTitleModel()
	{
		return titleModel;
	}
	
	/**
	 * Returns the application window width as recorded
	 * in the persistent user preferences.
	 * 
	 * @return application window width from user preferences
	 */
	int getWindowWidth()
	{
		return Integer.parseInt(preferences.get(WIDTH_KEY, "700"));
	}
	
	/**
	 * Returns the application window height as recorded
	 * in the persistent user preferences.
	 * 
	 * @return application window height from user preferences
	 */
	int getWindowHeight()
	{
		return Integer.parseInt(preferences.get(HEIGHT_KEY, "650"));
	}
	
	/**
	 * Updates the window title to display the newly
	 * activated session name.
	 * 
	 * @param event session activated event
	 */
	@Subscribe
	public void sessionActivated(SessionActivatedEvent event)
	{
		updateWindowTitle(event.getSession().getName(),
			event.getSession().isDirty());
	}
	
	/**
	 * Updates the window title to reflect the
	 * current state of the active session.
	 * 
	 * @param event session state event
	 */
	@Subscribe
	public void sessionUpdated(SessionStateEvent event)
	{
		updateWindowTitle(event.getSessionName(),
			event.isDirty());
	}
	
	/**
	 * Updates the application window title to reflect
	 * the currently active session.
	 * 
	 * @param sessionName
	 * @param dirty
	 */
	private void updateWindowTitle(String sessionName, boolean dirty)
	{
		StringBuilder title = new StringBuilder();
		title.append(Application.TITLE + " :: ");
		title.append(sessionName);
		
		if (dirty)
		{
			title.append("*");
		}
		
		titleModel.setTitle(title.toString());
	}

}
