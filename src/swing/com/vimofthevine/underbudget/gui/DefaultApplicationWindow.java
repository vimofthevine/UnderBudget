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

import java.awt.BorderLayout;
import java.awt.Component;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.Subscribe;

/**
 * Primary application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultApplicationWindow implements ApplicationWindow {

	/**
	 * Application window
	 */
	private final JFrame window;
	
	/**
	 * Constructs a new application window.
	 * 
	 * @param presentationModel application window presentation model
	 * @param frame             application window frame
	 * @param menu              application menu bar
	 * @param toolBar           application tool bar
	 * @param content           application window content
	 */
	public DefaultApplicationWindow(ApplicationWindowModel presentationModel,
		JFrame frame, JMenuBar menu, JToolBar toolBar, Component content)
	{
		window = frame;
		window.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		window.setLocationByPlatform(true);
		
		window.addComponentListener(presentationModel.getComponentListener());
		window.addWindowListener(presentationModel.getWindowListener());
		
		// Use a border layout so toolbar is rendered correctly
		window.setLayout(new BorderLayout());
		window.add(toolBar, BorderLayout.NORTH);
		window.add(content, BorderLayout.CENTER);
		window.setJMenuBar(menu);
		
		window.pack();
		// Set initial size
		window.setSize(
			presentationModel.getWindowWidth(),
			presentationModel.getWindowHeight());
	}

	@Override
	public final void setWindowTitle(final String title)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				window.setTitle(title);
			}
		});
	}
	
	@Override
	public final void setWindowSize(final int width, final int height)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				window.setSize(width, height);
			}
		});
	}
	
	@Override
	public final void display()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				window.setVisible(true);
			}
		});
	}
	
	@Override
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
