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

package com.vimofthevine.underbudget.swing.transaction.wizard;

import java.awt.Frame;
import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

import com.vimofthevine.underbudget.core.transaction.source.GnuCashXmlFileSource;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * Selection wizard for selecting a GnuCash
 * XML file from which to import transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GnuCashXmlFileWizard implements SourceWizard {

	/**
	 * Prompts the user for the location of
	 * the GnuCash XML file.
	 * 
	 * @param window application main window
	 * @param wizard transaction source selection wizard
	 * @param prefs  user preferences
	 */
	@Override
	public void select(final Frame window,
		final TransactionSourceSelectionWizard wizard,
		UserPreferences prefs)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				GnuCashXmlFileChooser chooser = new GnuCashXmlFileChooser();
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
    							GnuCashXmlFileSource source =
    								new GnuCashXmlFileSource(file);
    							wizard.sourceSelected(source);
							}
							catch (FileNotFoundException fnfe)
							{
							}
						}
					}.start();
				}
			}
		});
	}
	
}
