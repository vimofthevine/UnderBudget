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

package com.vimofthevine.underbudget.swing.budget.period;

import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JComboBox;
import javax.swing.JSpinner;
import javax.swing.JTextField;

/**
 * View for defining a monthly budgeting period.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class MonthFormView {
	
	/**
	 * Constructs a new month form view.
	 * 
	 * @param container Swing component for this view
	 * @param model     month form view model
	 */
	public MonthFormView(Container container,
		MonthFormViewModel model)
	{
		JTextField summary = new JTextField();
		summary.setDocument(model.getSummaryDocument());
		summary.setEditable(false);
		
		JComboBox month = new JComboBox(model.getMonthModel());
		JSpinner year = new JSpinner(model.getYearModel());
		year.setEditor(new JSpinner.NumberEditor(year, "#"));
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c;
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridwidth = 2;
		c.insets = new Insets(0, 5, 5, 5);
		container.add(summary, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 1;
		c.insets = new Insets(0, 5, 5, 5);
		c.weightx = 1.0;
		c.weighty = 1.0;
		container.add(month, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = 1;
		c.gridy = 1;
		c.insets = new Insets(0, 0, 5, 5);
		container.add(year, c);
	}

}
