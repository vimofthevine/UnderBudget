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

package com.vimofthevine.underbudget.gui.status;

import java.awt.BorderLayout;
import java.awt.Container;

import javax.swing.JLabel;
import javax.swing.JProgressBar;
import javax.swing.SwingUtilities;

/**
 * Status bar to display current tasks
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StatusBar {

	/**
	 * Status text
	 */
	private final JLabel text;
	
	/**
	 * Status progress bar
	 */
	private final JProgressBar progress;
	
	/**
	 * Constructs a new status bar.
	 * 
	 * @param status status bar container
	 */
	public StatusBar(Container status)
	{
		status.setLayout(new BorderLayout());
		
		text = new JLabel("Ready");
		progress = new JProgressBar(0, 100);
		
		status.add(text, BorderLayout.WEST);
		status.add(progress, BorderLayout.EAST);
	}
	
	/**
	 * Updates the text displayed in the status bar.
	 * 
	 * @param status new status text
	 */
	public void displayMessage(final String status)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				text.setText(status);
			}
		});
	}
	
	/**
	 * Updates the progress displayed in the status bar.
	 * 
	 * @param value new progress value
	 */
	public void displayProgress(final int value)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				progress.setValue(value);
			}
		});
	}
	
}
