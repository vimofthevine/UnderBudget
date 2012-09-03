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
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.SwingUtilities;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * Dialog prompting the user to select the type of
 * budget source.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SourceTypeSelectionDialog {

	/**
	 * Constructs a new source type selection dialog.
	 * 
	 * @param window application window
	 * @param wizard budget source selection wizard
	 * @param event  original select-source event
	 */
	public SourceTypeSelectionDialog(final Frame window,
		final BudgetSourceSelectionWizard wizard,
		final Object selectionEvent)
	{
		JPanel panel = new JPanel(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridwidth = 2;
		c.gridx = 0;
		c.gridy = GridBagConstraints.RELATIVE;
		c.insets = new Insets(5, 5, 5, 5);
		panel.add(new BoldLabel("Select budget source"), c);
		
		final JButton next = new JButton("Next");
		next.setEnabled(false);
		
		c.insets = new Insets(5, 5, 0, 5);
		final ButtonGroup group = new ButtonGroup();
		for (SourceType type : SourceType.values())
		{
			JRadioButton button = new JRadioButton(type.toString());
			button.setActionCommand(type.getActionCommand());
			group.add(button);
			panel.add(button, c);
			
			// Enable the next button
			button.addActionListener(new ActionListener() {
				@Override
                public void actionPerformed(ActionEvent arg0)
                {
					next.setEnabled(true);
                }
			});
		}
		
		JButton cancel = new JButton("Cancel");
		
		c.fill = GridBagConstraints.NONE;
		c.anchor = GridBagConstraints.LAST_LINE_START;
		c.gridwidth = 1;
		c.gridx = 0;
		c.insets = new Insets(5, 5, 5, 5);
		c.weighty = 1.0;
		panel.add(cancel, c);
		
		c.anchor = GridBagConstraints.LAST_LINE_END;
		c.gridx = 1;
		panel.add(next, c);
		
		final JDialog dialog = new JDialog(window, "Select budget source", true);
		dialog.getContentPane().add(panel);
		
		cancel.addActionListener(new ActionListener() {
			@Override
            public void actionPerformed(ActionEvent event)
            {
				dialog.dispose();
            }
		});
		
		next.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				String command = group.getSelection().getActionCommand();
				final SourceType type = SourceType.valueOf(command);
				dialog.dispose();
				
				// Get off EDT
				new Thread() {
					public void run()
					{
						wizard.typeSelected(type, selectionEvent);
					}
				}.start();
			}
		});
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.pack();
				dialog.setLocationRelativeTo(window);
				dialog.setVisible(true);
			}
		});
	}
	
}
