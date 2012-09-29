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

package com.vimofthevine.underbudget.swing.session.wizard;

import java.awt.Frame;
import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;
import java.io.File;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToOpenSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToSaveSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToOpenEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToSaveEvent;
import com.vimofthevine.underbudget.xml.budget.source.AesEncryptedFileSource;
import com.vimofthevine.underbudget.xml.budget.source.BudgetXmlFileSource;

/**
 * Wizard for selecting a budget to be opened.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceSelectionWizard {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetSourceSelectionWizard.class.getName());
	
	/**
	 * Last-opened directory preferences key
	 */
	private static final String LAST_OPENED = "LastOpenedBudgetDir.";
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * Current budget
	 */
	private final Budget budget;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Constructs a new budget source selection wizard.
	 * 
	 * @param bus    event bus
	 * @param parent application window
	 * @param budget budget
	 */
	public BudgetSourceSelectionWizard(EventBus bus,
		Frame parent, Budget budget, UserPreferences prefs)
	{
		eventBus = bus;
		eventBus.register(this);
		
		window = parent;
		
		this.budget = budget;
		
		preferences = prefs;
	}
	
	@Subscribe
	public void selectSourceToOpen(SelectBudgetSourceToOpenEvent event)
	{
		logger.log(Level.FINE, "Prompting for source to open");
		new SourceTypeSelectionDialog(window, this, event);
	}
	
	@Subscribe
	public void selectSourceToSave(SelectBudgetSourceToSaveEvent event)
	{
		logger.log(Level.FINE, "Prompting for source to save");
		new SourceTypeSelectionDialog(window, this, event);
	}
	
	void typeSelected(SourceType type, final Object event)
	{
		logger.log(Level.FINER, type + " type has been selected for " + event);
		
		switch (type)
		{
			case XML:
				selectGnuCashXmlFile(event);
				break;
				
			case AES:
				selectAesEncryptedFile(event);
				break;
				
			default:
				// Do nothing
		}
	}
	
	/**
	 * Fires a source-selected event, according to the
	 * type of the original select-source event.
	 * 
	 * @param source budget source selected
	 * @param event  original select-source event
	 */
	private void fireSelectedEvent(BudgetSource source, Object event)
	{
		if (event instanceof SelectBudgetSourceToOpenEvent)
		{
			eventBus.post(new BudgetSourceToOpenSelectedEvent(source));
		}
		else
		{
			eventBus.post(new BudgetSourceToSaveSelectedEvent(source));
		}
	}
	
	private void selectGnuCashXmlFile(final Object event)
	{
		final boolean open = (event instanceof SelectBudgetSourceToOpenEvent);
		final String directory = preferences.get(LAST_OPENED + "XML", "");
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				BudgetXmlFileChooser chooser = new BudgetXmlFileChooser(directory);
				int result = open ? chooser.showOpenDialog(window)
					: chooser.showSaveDialog(window);
				
				if (result == JFileChooser.APPROVE_OPTION)
				{
					final File file = chooser.getSelectedFile();
					preferences.set(LAST_OPENED + "XML",
						chooser.getCurrentDirectory().getAbsolutePath());
					
					// Get off EDT
					new Thread() {
						public void run()
						{
							BudgetXmlFileSource source = open
								? new BudgetXmlFileSource(file)
								: new BudgetXmlFileSource(file, budget);
							
							fireSelectedEvent(source, event);
						}
					}.start();
				}
			}
		});
	}
	
	private void selectAesEncryptedFile(final Object event)
	{
		final boolean open = (event instanceof SelectBudgetSourceToOpenEvent);
		final String directory = preferences.get(LAST_OPENED + "AES", "");
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				AesEncryptedFileChooser chooser = new AesEncryptedFileChooser(directory);
				int result = open ? chooser.showOpenDialog(window)
					: chooser.showSaveDialog(window);
				
				if (result == JFileChooser.APPROVE_OPTION)
				{
					final File file = chooser.getSelectedFile();
					preferences.set(LAST_OPENED + "AES",
						chooser.getCurrentDirectory().getAbsolutePath());
					
					// Prompt for password
					final String key = promptForPassword("Password");
					
					if (key == null || key.equals("")) // User hit cancel/empty password
						return;
					
					if ( ! open) // save
					{
						// Have user confirm password
						String confirm = promptForPassword("Confirm password");
						if ( ! key.equals(confirm))
						{
							JOptionPane.showMessageDialog(window,
								"Passwords do not match", "Incorrect password",
								JOptionPane.ERROR_MESSAGE);
							return;
						}
					}
					
					// Get off EDT
					new Thread() {
						public void run()
						{
    						AesEncryptedFileSource source = open
    							? new AesEncryptedFileSource(file, key)
    							: new AesEncryptedFileSource(file, key, budget);
    							
   							fireSelectedEvent(source, event);
						}
					}.start();
				}
			}
		});
	}
	
	/**
	 * Prompts the user for the encryption passkey.
	 * 
	 * @param prompt prompt text
	 * @return encryption passkey, or false if the user hits "cancel"
	 */
	private String promptForPassword(String prompt)
	{
		// Already on EDT
		JPanel panel = new JPanel();
		JLabel label = new JLabel(prompt);
		final JPasswordField password = new JPasswordField(10);
		panel.add(label);
		panel.add(password);
		
		password.addHierarchyListener(new HierarchyListener() {
			@Override
            public void hierarchyChanged(final HierarchyEvent event)
            {
				final HierarchyListener listener = this;
				if ((HierarchyEvent.SHOWING_CHANGED & event.getChangeFlags()) != 0
					&& password.isShowing())
				{
					Timer timer = new Timer();
					timer.schedule(new TimerTask() {
						public void run()
						{
							SwingUtilities.invokeLater(new Runnable() {
								public void run()
								{
                					password.requestFocusInWindow();
                					password.removeHierarchyListener(listener);
								}
							});
						}
					}, 100);
				}
            }
		});
		
		int result = JOptionPane.showOptionDialog(window, panel, "Encryption Password",
			JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE,
			null, null, "");
		
		if (result == JOptionPane.OK_OPTION)
		{
			char[] passkey = password.getPassword();
			return new String(passkey);
		}
		else
			return null;
	}

}
