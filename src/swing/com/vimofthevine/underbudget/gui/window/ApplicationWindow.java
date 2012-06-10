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

import java.awt.BorderLayout;
import java.awt.Component;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.gui.ApplicationShutdownEvent;

/**
 * Primary application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ApplicationWindow {

	/**
	 * Application window
	 */
	private final JFrame window;
	
	/**
	 * Constructs a new application window.
	 * 
	 * @param model   application window presentation model
	 * @param frame   application window frame
	 * @param menu    application menu bar
	 * @param toolBar application tool bar
	 * @param content application window content
	 */
	public ApplicationWindow(ApplicationWindowModel model,
		JFrame frame, JMenuBar menu, JToolBar toolBar, Component content)
	{
		window = frame;
		window.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		window.setLocationByPlatform(true);
		
		window.addComponentListener(model.getComponentListener());
		window.addWindowListener(model.getWindowListener());
		
		// Use a border layout so toolbar is rendered correctly
		window.setLayout(new BorderLayout());
		window.add(toolBar, BorderLayout.NORTH);
		window.add(content, BorderLayout.CENTER);
		window.setJMenuBar(menu);
		
		window.setTitle(model.getTitleModel().getTitle());
		model.getTitleModel().addWindowTitleListener(
			new WindowTitleListener() {
				public void titleChanged(final String title)
				{
					SwingUtilities.invokeLater(new Runnable() {
						public void run()
						{
							window.setTitle(title);
						}
					});
				}
			}
		);
		
		window.pack();
		// Set initial size
		window.setSize(
			model.getWindowWidth(),
			model.getWindowHeight());
	}

	public final void display()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				window.setVisible(true);
			}
		});
	}
	
	public final void dispose()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				window.setVisible(false);
				window.dispose();
			}
		});
	}
	
	@Subscribe
	public final void handleShutdown(ApplicationShutdownEvent event)
	{
		dispose();
	}
	
}
