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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.ApplicationProperties;

/**
 * Popup dialog to display information about the application.
 * This is currently implemented as an autonomous dialog.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AboutDialog implements ActionListener {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(AboutDialog.class.getName());
	
	/**
	 * About dialog
	 */
	private final JDialog dialog;
	
	/**
	 * Parent frame
	 */
	private final Frame parent;
	
	/**
	 * Constructs a new about dialog.
	 * 
	 * @param props  application properties
	 * @param parent parent window to the popup dialog
	 * @param bus    application event bus
	 */
	public AboutDialog(ApplicationProperties props,
		Frame parent, EventBus bus)
	{
		bus.register(this);
		
		this.parent = parent;
		dialog = new JDialog(parent, "About " + props.getTitle(), true);
		dialog.setLayout(new GridBagLayout());
		
		// Create tabs
		JTabbedPane tabs = new JTabbedPane();
		tabs.addTab("About", createAboutTab());
		tabs.addTab("Acknowledgements", createAcknowledgementsTab());
		tabs.addTab("License", createLicenseTab());
		tabs.setPreferredSize(new Dimension(550, 250));
		
		// Create close button
		JButton close = new JButton("Close");
		close.setPreferredSize(new Dimension(80, 25));
		close.setActionCommand("CLOSE");
		close.addActionListener(this);
		
		// Add tabs to dialog
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.BOTH;
		c.weightx = 1.0;
		c.weighty = 1.0;
		dialog.add(tabs, c);
		
		// Add close button to dialog
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridy = 1;
		c.insets = new Insets(5, 0, 5, 5);
		dialog.add(close, c);
		
		dialog.pack();
	}
	
	private Component createAboutTab()
	{
		JEditorPane pane = new JEditorPane();
		pane.setEditable(false);
		
		URL about = getClass().getResource("about/about.html");
		if (about != null)
		{
			try
			{
				pane.setPage(about);
			}
			catch (IOException ioe)
			{
				logger.log(Level.WARNING, "Error loading about page", ioe);
			}
		}
		
		return new JScrollPane(pane);
	}
	
	private Component createAcknowledgementsTab()
	{
		JEditorPane pane = new JEditorPane();
		pane.setEditable(false);
		
		URL about = getClass().getResource("about/acknowledgements.html");
		if (about != null)
		{
			try
			{
				pane.setPage(about);
			}
			catch (IOException ioe)
			{
				logger.log(Level.WARNING, "Error loading about page", ioe);
			}
		}
		
		return new JScrollPane(pane);
	}
	
	private Component createLicenseTab()
	{
		JEditorPane pane = new JEditorPane();
		pane.setEditable(false);
		
		URL about = getClass().getResource("about/license.html");
		if (about != null)
		{
			try
			{
				pane.setPage(about);
			}
			catch (IOException ioe)
			{
				logger.log(Level.WARNING, "Error loading about page", ioe);
			}
		}
		
		return new JScrollPane(pane);
	}
	
	/**
	 * Displays the about popup dialog.
	 */
	public void display()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.setLocationRelativeTo(parent);
				dialog.setVisible(true);
			}
		});
	}
	
	@Subscribe
	public void display(DisplayAboutEvent event)
	{
		display();
	}
	
	/**
	 * Disposes of the about dialog.
	 */
	public void dispose()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.setVisible(false);
			}
		});
	}
	
	@Override
	public void actionPerformed(ActionEvent event)
	{
		if (event.getActionCommand().equals("CLOSE"))
		{
			dispose();
		}
	}

}
