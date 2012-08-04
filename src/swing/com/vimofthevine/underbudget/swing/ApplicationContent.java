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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.net.URL;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.swing.session.events.SessionActivatedEvent;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ApplicationContent {
	
	/**
	 * Application content view
	 */
	private final Container view;
	
	/**
	 * Constructs a new application content instance.
	 * 
	 * @param container application content container
	 */
	public ApplicationContent(Container container, EventBus bus)
	{
		bus.register(this);
		
		view = container;
		view.setLayout(new BorderLayout());
		
		view.add(createSplashComponent(), BorderLayout.CENTER);
	}
	
	/**
	 * Creates a component to display a large
	 * application logo in the absence of any
	 * opened session content.
	 * 
	 * @return application logo component
	 */
	private Component createSplashComponent()
	{
		URL url = getClass().getResource("splash.png");
		return (url != null)
			? new JLabel(new ImageIcon(url))
			: new JLabel(Application.TITLE);
	}

	/**
	 * Updates the application content view to display
	 * the content for the active session.
	 * 
	 * @param event session activated event
	 */
	@Subscribe
	public void sessionActivated(SessionActivatedEvent event)
	{
		final Component component = (event.getSession() != null)
			? event.getSession().getComponent()
			: createSplashComponent();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				view.removeAll();
				view.add(component, BorderLayout.CENTER);
				view.validate();
			}
		});
	}
	
}
