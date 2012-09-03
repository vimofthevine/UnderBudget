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

import javax.swing.SwingUtilities;

import com.google.common.eventbus.Subscribe;

/**
 * Status bar presentation model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StatusBarModel {

	/**
	 * Status bar
	 */
	private final StatusBar status;
	
	/**
	 * Constructs a new status bar model.
	 * 
	 * @param status status bar
	 */
	public StatusBarModel(StatusBar status)
	{
		this.status = status;
	}
	
	@Subscribe
	public void updateStatus(final StatusEvent event)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				if (event.getMessage() != null)
				{
					status.displayMessage(event.getMessage());
				}
				
				if (event.getProgress() != -1)
				{
					status.displayProgress(event.getProgress());
				}
			}
		});
	}
	
}
