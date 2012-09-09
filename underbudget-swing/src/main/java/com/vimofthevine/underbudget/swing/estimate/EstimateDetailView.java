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

package com.vimofthevine.underbudget.swing.estimate;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import java.awt.Container;

import javax.swing.JButton;

import org.jdesktop.swingx.JXDatePicker;

import com.vimofthevine.underbudget.swing.currency.CurrencyField;
import com.vimofthevine.underbudget.swing.widgets.BoldLabel;
import com.vimofthevine.underbudget.swing.widgets.CheckboxInputField;
import com.vimofthevine.underbudget.swing.widgets.ComboInputField;
import com.vimofthevine.underbudget.swing.widgets.TextInputField;

/**
 * View for displaying modifiable details about
 * a specific estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDetailView {

	/**
	 * Constructs a new estimate detail view.
	 * 
	 * @param container Swing component for this view
	 * @param model     estimate detail view model
	 */
	public EstimateDetailView(Container container,
		EstimateDetailViewModel model)
	{
		TextInputField name = new TextInputField(model.getNameModel());
		TextInputField description = new TextInputField(model.getDescriptionModel());
		CurrencyField amount = new CurrencyField(model.getAmountModel()); 
		ComboInputField type = new ComboInputField(model.getTypeModel());
		CheckboxInputField complete = new CheckboxInputField(model.getCompleteModel());
		
		JXDatePicker dueDate = new JXDatePicker();
		dueDate.getMonthView().setSelectionModel(model.getDateModel());
		
		JButton addButton = new JButton(model.getAddChildAction());
		JButton deleteButton = new JButton(model.getDeleteAction());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c;
		int row = -1;
		int col = -1;
		
		// 1st row
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = ++row;
		container.add(new BoldLabel("Name"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = ++col;
		c.gridy = row;
		c.weightx = 1.0;
		container.add(name, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(new BoldLabel("Type"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(type, c);
		
		// 2nd row
		col = -1;
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = ++row;
		container.add(new BoldLabel("Description"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = ++col;
		c.gridy = row;
		c.weightx = 1.0;
		container.add(description, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(new BoldLabel("Due Date"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(dueDate, c);
		
		// 3rd row
		col = -1;
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = ++row;
		container.add(new BoldLabel("Amount"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = ++col;
		c.gridy = row;
		c.weightx = 1.0;
		container.add(amount, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(new BoldLabel("Complete?"), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(complete, c);
		
		// 4th row
		col = -1;
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = ++row;
		container.add(addButton, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		c.gridx = ++col;
		c.gridy = row;
		container.add(deleteButton, c);
		
	}
	
}
