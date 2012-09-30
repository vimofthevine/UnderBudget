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

package com.vimofthevine.underbudget.swing.session.source.xml;

import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.session.recent.RecentBudgetXmlFile;
import com.vimofthevine.underbudget.swing.session.source.SelectSourceForAction;
import com.vimofthevine.underbudget.swing.session.source.SourceType;
import com.vimofthevine.underbudget.xml.budget.source.BudgetXmlFileFactory;

/**
 * Wizard for selecting a budget XML file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetXmlFileWizard {

	/**
	 * Last-opened directory preferences key
	 */
	private static final String LAST_OPENED = "LastOpenedBudgetDir.XML";

	@Subscribe
	public void selectFile(final SelectSourceForAction event)
	{
		if (event.getSourceType() != SourceType.XML)
			return;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				File file = promptUserForFile(event);
				
				if (file != null)
				{
					sendSourceEvent(event, file);
				}
			}
		});
	}
	
	private File promptUserForFile(SelectSourceForAction event)
	{
		String directory = event.getPreferences().get(LAST_OPENED, "");
		BudgetXmlFileChooser chooser = new BudgetXmlFileChooser(directory);
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
		final File file)
	{
		// Get off EDT
		new Thread() {
			public void run()
			{
				BudgetXmlFileFactory factory
					= new BudgetXmlFileFactory(file);
   				RecentBudgetXmlFile session
   					= new RecentBudgetXmlFile(file);
   				
  				event.select(factory, session);
			}
		}.start();
	}
	
}
