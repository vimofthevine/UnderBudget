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

package com.vimofthevine.underbudget.swing.budget;

import java.awt.Component;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import com.vimofthevine.underbudget.swing.currency.CurrencyField;
import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View for displaying modifiable fields of
 * the current budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class EditBudgetView {
	
	public static void display(final Frame parent,
		EditBudgetViewModel model, Component period)
	{
		JTextField nameField = new JTextField();
		nameField.setDocument(model.getNameDocument());
		
		CurrencyField initialField = new CurrencyField(
			model.getInitialBalanceDocument());
		
		JButton closeButton = new JButton(model.getCloseAction());
		
		final JDialog dialog = new JDialog(parent, "Edit Budget");
		dialog.setModal(true);
		
		closeButton.addActionListener(new ActionListener() {
			@Override
            public void actionPerformed(ActionEvent arg0)
            {
				dialog.dispose();
            }
		});
		
		dialog.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.PAGE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = GridBagConstraints.RELATIVE;
		c.insets = new Insets(2, 5, 0, 5);
		c.weightx = 1.0;
		
		dialog.add(new BoldLabel("Budget Name"), c);
		dialog.add(nameField, c);
		dialog.add(new BoldLabel("Initial Balance"), c);
		dialog.add(initialField, c);
		dialog.add(new BoldLabel("Date Period"), c);
		dialog.add(period, c);
		
		c.anchor = GridBagConstraints.LAST_LINE_END;
		c.insets = new Insets(2, 5, 5, 5);
		c.weighty = 1.0;
		dialog.add(closeButton, c);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.pack();
				dialog.setLocationRelativeTo(parent);
				dialog.setVisible(true);
			}
		});
	}
	
}
