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

package com.vimofthevine.underbudget.swing;

import java.awt.Frame;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.DeadEvent;
import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;

/**
 * A <code>DeadEventListener</code> catches all dead
 * events, displaying a simple message to the user
 * that the requested feature has not yet been implemented.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DeadEventListener {
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * Constructs a new dead event listener.
	 * 
	 * @param window application window
	 * @param bus    application event bus
	 */
	public DeadEventListener(Frame window, EventBus bus)
	{
		this.window = window;
		bus.register(this);
	}

	/**
	 * Upon receipt of a dead event, an info dialog
	 * is shown to the user indicating that the requested
	 * feature is unimplemented.
	 * 
	 * @param event dead event
	 */
	@Subscribe
	public void deadEventPosted(final DeadEvent event)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
        		JOptionPane.showMessageDialog(window,
        			"The requested feature has yet to be implemented:\n" +
        			getFeatureNameFromEventName(event.getEvent()),
        			"Unimplemented Feature", JOptionPane.INFORMATION_MESSAGE);
			}
		});
	}
	
	private String getFeatureNameFromEventName(Object event)
	{
		String eventName = event.getClass().getSimpleName();
		eventName = eventName.replace("Event", "");
		return eventName.replaceAll(
			String.format("%s|%s|%s",
				"(?<=[A-Z])(?=[A-Z][a-z])",
				"(?<=[^A-Z])(?=[A-Z])",
				"(?<=[A-Za-z])(?=[^A-Za-z])"
			), " "
		);
	}
	
}
