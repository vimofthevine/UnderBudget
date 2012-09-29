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

package com.vimofthevine.underbudget.swing.status;

import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for the application window
 * status bar.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class StatusBarViewModel {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(StatusBarViewModel.class.getName());
	
	/**
	 * Status message document model
	 */
	private final SimpleDocument messageModel;
	
	/**
	 * Progress bar model
	 */
	private final ProgressModel progressModel;
	
	/**
	 * Timer for temporary messages
	 */
	private Timer timer;
	
	/**
	 * Constructs a new status bar view model.
	 * 
	 * @param bus event bus
	 */
	StatusBarViewModel(EventBus bus)
	{
		bus.register(this);
		
		messageModel = new SimpleDocument();
		messageModel.setText("Ready");
		progressModel = new ProgressModel();
	}
	
	/**
	 * Returns the document model for status messages.
	 * 
	 * @return status message document model
	 */
	Document getMessageModel()
	{
		return messageModel;
	}
	
	/**
	 * Returns the range model for progress updates.
	 * 
	 * @return progress range model
	 */
	ProgressModel getProgressModel()
	{
		return progressModel;
	}
	
	@Subscribe
	public void displayMessage(StatusMessageEvent event)
	{
		logger.log(Level.FINER, "Received status message event, " + event);
		final String message = event.getMessage();
		
		// Cancel the existing timer (if any)
		if (timer != null)
		{
			timer.cancel();
		}
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				messageModel.setText(message);
			}
		});
		
		if (event.getDuration() > 0)
		{
			timer = new Timer("Status message timeout timer");
			timer.schedule(new TimerTask() {
				public void run()
				{
					SwingUtilities.invokeLater(new Runnable() {
						public void run()
						{
							messageModel.setText("");
						}
					});
				}
			}, event.getDuration());
		}
	}
	
	@Subscribe
	public void updateProgress(final ProgressEvent event)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				progressModel.setIndeterminate(event.isIndeterminate());
				
				if ( ! event.isIndeterminate())
				{
					progressModel.setValue(event.getValue());
				}
			}
		});
	}
	
}
