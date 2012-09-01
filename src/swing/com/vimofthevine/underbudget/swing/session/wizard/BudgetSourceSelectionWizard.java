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
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.stubs.budget.source.StubBudgetSource;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToOpenSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.BudgetSourceToSaveSelectedEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToOpenEvent;
import com.vimofthevine.underbudget.swing.session.events.SelectBudgetSourceToSaveEvent;

/**
 * Wizard for selecting a budget to be opened.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceSelectionWizard {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Application window
	 */
	private final Frame window;
	
	/**
	 * Constructs a new budget source selection wizard.
	 * 
	 * @param bus    event bus
	 * @param parent application window
	 */
	public BudgetSourceSelectionWizard(EventBus bus,
		Frame parent)
	{
		eventBus = bus;
		eventBus.register(this);
		
		window = parent;
	}
	
	@Subscribe
	public void selectSourceToOpen(SelectBudgetSourceToOpenEvent event)
	{
		new SourceTypeSelectionDialog(window, this, event);
	}
	
	@Subscribe
	public void selectSourceToSave(SelectBudgetSourceToSaveEvent event)
	{
		new SourceTypeSelectionDialog(window, this, event);
	}
	
	void typeSelected(SourceType type, final Object event)
	{
		switch (type)
		{
			case XML:
				SwingUtilities.invokeLater(new Runnable() {
					public void run()
					{
						BudgetXmlFileChooser chooser = new BudgetXmlFileChooser();
						int result = chooser.showOpenDialog(window);
						
						if (result == JFileChooser.APPROVE_OPTION)
						{
							final File file = chooser.getSelectedFile();
							
							// Get off EDT
							new Thread() {
								public void run()
								{
									//BudgetXmlFileSource source =
										//new BudgetXmlFileSource(file);
									//fireSelectedEvent(source, event);
								}
							}.start();
						}
					}
				});
				
				break;
				
			case STUB:
				fireSelectedEvent(new StubBudgetSource(), event);
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

}
