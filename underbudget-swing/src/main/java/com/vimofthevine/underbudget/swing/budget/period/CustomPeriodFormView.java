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

import javax.swing.JTextField;

import org.jdesktop.swingx.JXDatePicker;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View for defining a custom budgeting period.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomPeriodFormView {
	
	public CustomPeriodFormView(Container container,
		CustomPeriodFormViewModel model)
	{
		JTextField summary = new JTextField();
		summary.setDocument(model.getSummaryDocument());
		summary.setEditable(false);
		
		JXDatePicker start = new JXDatePicker();
		start.getMonthView().setSelectionModel(model.getStartDateModel());
		JXDatePicker end = new JXDatePicker();
		end.getMonthView().setSelectionModel(model.getEndDateModel());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c;
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridwidth = 2;
		c.insets = new Insets(0, 5, 5, 5);
		container.add(summary, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_START;
		c.gridx = 0;
		c.gridy = 1;
		c.insets = new Insets(0, 5, 5, 5);
		container.add(new BoldLabel("Start:"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 1;
		c.gridy = 1;
		c.insets = new Insets(0, 0, 5, 5);
		container.add(start, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_START;
		c.gridx = 0;
		c.gridy = 2;
		c.insets = new Insets(0, 5, 5, 5);
		container.add(new BoldLabel("End:"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 1;
		c.gridy = 2;
		c.insets = new Insets(0, 0, 5, 5);
		container.add(end, c);
	}

}
