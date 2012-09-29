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

import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JProgressBar;
import javax.swing.JTextField;

/**
 * Status bar to display current tasks
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class StatusBarView implements ProgressListener {
	
	/**
	 * Status progress bar
	 */
	private final JProgressBar progress;
	
	/**
	 * Constructs a new status bar.
	 * 
	 * @param container Swing component for this view
	 * @param model     status bar view model
	 */
	StatusBarView(Container container, StatusBarViewModel model)
	{
		JTextField message = new JTextField();
		message.setDocument(model.getMessageModel());
		message.setEditable(false);
		message.setOpaque(false);
		message.setBorder(BorderFactory.createEmptyBorder());
		
		progress = new JProgressBar(0, 100);
		model.getProgressModel().addListener(this);
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		c.fill = GridBagConstraints.HORIZONTAL;
		c.insets = new Insets(2, 2, 2, 2);
		c.weightx = 1.0;
		container.add(message, c);
		
		c = new GridBagConstraints();
		c.gridx = 1;
		c.insets = new Insets(2, 0, 2, 2);
		container.add(progress, c);
	}
	
	@Override
    public void setIndeterminate(boolean indeterminate)
    {
		progress.setIndeterminate(indeterminate);
    }

	@Override
    public void setValue(int value)
    {
		if (value < 0)
		{
			value = 0;
		}
		
		if (value > 100)
		{
			value = 100;
		}
		
		progress.setValue(value);
    }
	
}
