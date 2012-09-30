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

package com.vimofthevine.underbudget.swing.session.wizard.aes;

import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.session.recent.RecentAesEncryptedFile;
import com.vimofthevine.underbudget.swing.session.wizard.SelectSourceForAction;
import com.vimofthevine.underbudget.swing.session.wizard.SourceType;
import com.vimofthevine.underbudget.swing.widgets.ErrorPopup;
import com.vimofthevine.underbudget.swing.widgets.PasswordPrompt;
import com.vimofthevine.underbudget.xml.budget.source.AesEncryptedFileFactory;

/**
 * Wizard for selecting an AES-encrypted file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AesEncryptedFileWizard {
	
	/**
	 * Last-opened directory preferences key
	 */
	private static final String LAST_OPENED = "LastOpenedBudgetDir.AES";
	
	@Subscribe
	public void selectFile(final SelectSourceForAction event)
	{
		if (event.getSourceType() != SourceType.AES)
			return;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				File file = promptUserForFile(event);
				
				if (file != null)
				{
					// Prompt for password
					final String key = PasswordPrompt.prompt("Password",
						event.getParentWindow());
					
					// User hit cancel/empty password
					if (key == null || key.equals(""))
						return;
					
					if ( ! event.isOpenEvent()) // save
					{
						// Have user confirm password
						String confirm = PasswordPrompt.prompt("Confirm password",
							event.getParentWindow());
						
						if ( ! key.equals(confirm))
						{
							JOptionPane.showMessageDialog(event.getParentWindow(),
								"Passwords do not match", "Incorrect password",
								JOptionPane.ERROR_MESSAGE);
							return;
						}
					}
					
					sendSourceEvent(event, file, key);
				}
			}
		});
	}
	
	private File promptUserForFile(SelectSourceForAction event)
	{
		String directory = event.getPreferences().get(LAST_OPENED, "");
		AesEncryptedFileChooser chooser = new AesEncryptedFileChooser(directory);
		int result = event.isOpenEvent() ? chooser.showOpenDialog(event.getParentWindow())
			: chooser.showSaveDialog(event.getParentWindow());
				
		if (result == JFileChooser.APPROVE_OPTION)
		{
			final File file = chooser.getSelectedFile();
			event.getPreferences().set(LAST_OPENED,
				chooser.getCurrentDirectory().getAbsolutePath());
					
			return file;
		}
		
		return null;
	}
	
	private void sendSourceEvent(final SelectSourceForAction event,
		final File file, final String key)
	{
		// Get off EDT
		new Thread() {
			public void run()
			{
				try
				{
					AesEncryptedFileFactory factory
						= new AesEncryptedFileFactory(file, key);
    				RecentAesEncryptedFile session
    					= new RecentAesEncryptedFile(file);
    				
    				event.select(factory, session);
				}
				catch (FileNotFoundException fnfe)
				{
					new ErrorPopup(fnfe, event.getParentWindow());
				}
			}
		}.start();
	}

}
