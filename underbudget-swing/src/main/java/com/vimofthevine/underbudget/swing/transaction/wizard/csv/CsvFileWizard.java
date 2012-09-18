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

package com.vimofthevine.underbudget.swing.transaction.wizard.csv;

import java.awt.Frame;
import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.transaction.source.CsvFileSource;
import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfile;
import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfileList;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.transaction.wizard.SourceWizard;
import com.vimofthevine.underbudget.swing.transaction.wizard.TransactionSourceSelectionWizard;

/**
 * Wizard for selecting a CSV profile and a CSV file
 * as a transaction source.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileWizard implements SourceWizard {
	
	/**
	 * Event bus
	 */
	private EventBus bus;
	
	/**
	 * CSV profile dialog
	 */
	private JDialog profileDialog;
	
	/**
	 * Application window
	 */
	private Frame window;
	
	/**
	 * Transaction source selection wizard
	 */
	private TransactionSourceSelectionWizard wizard;

	@Override
    public void select(final Frame window,
    	final TransactionSourceSelectionWizard wizard,
    	final UserPreferences prefs)
    {
		// Don't do anything if a wizard is already open
		if (bus != null)
			return;
		
		bus = new EventBus();
		bus.register(this);
		
		this.window = window;
		this.wizard = wizard;
		
		CsvProfileList profileList = new CsvProfileList(prefs.get("HOME", "."));
		
		CsvProfileListViewModel listModel
			= new CsvProfileListViewModel(bus, profileList);
		JPanel listComponent = new JPanel();
		new CsvProfileListView(listComponent, listModel);
		
		profileDialog = new JDialog(window,"Select CSV profile", true);
		profileDialog.getContentPane().add(listComponent);
		profileDialog.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				profileDialog.pack();
				profileDialog.setLocationRelativeTo(window);
				profileDialog.setVisible(true);
			}
		});
    }
	
	@Subscribe
	public void profileChosen(ProfileChosenEvent event)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				profileDialog.dispose();
			}
		});
		
		CsvProfile profile = event.getProfile();
		// If profile is null, cancel was hit
		if (profile != null)
		{
			selectFile(window, profile, wizard);
		}
		else
		{
			bus = null;
		}
	}
	
	/**
	 * Prompts the user to select the actual CSV file to be opened.
	 * 
	 * @param window  application window
	 * @param profile CSV profile to be used
	 * @param wizard  transaction source selection wizard
	 */
	void selectFile(final Frame window, final CsvProfile profile,
		final TransactionSourceSelectionWizard wizard)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				CsvFileChooser chooser = new CsvFileChooser();
				int result = chooser.showOpenDialog(window);
				
				if (result == JFileChooser.APPROVE_OPTION)
				{
					final File file = chooser.getSelectedFile();
					
					// Get off EDT
					new Thread() {
						public void run()
						{
							try
							{
								CsvFileSource source =
									new CsvFileSource(file, profile);
								wizard.sourceSelected(source);
							}
							catch (FileNotFoundException fnfe)
							{
							}
						}
					}.start();
				}
				
				bus = null;
			}
		});

	}
	
}
