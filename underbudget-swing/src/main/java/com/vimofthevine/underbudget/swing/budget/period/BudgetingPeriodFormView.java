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

import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JComboBox;
import javax.swing.JPanel;

import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * Autonomous view for selecting the type of
 * budgeting period to be defined.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class BudgetingPeriodFormView implements ActionListener {
	
	/**
	 * Container for all form views
	 */
	private final Container view;
	
	/**
	 * Form view container layout
	 */
	private final CardLayout layout;
	
	/**
	 * Type selection drop-down
	 */
	private final JComboBox typeSelection;
	
	/**
	 * Budgeting period form view models
	 */
	private final Map<PeriodType, BudgetingPeriodFormViewModel> forms;
	
	/**
	 * Flag to check whether the period type selection was
	 * changed programmatically or by the user.
	 */
	private boolean changedByApi = false;
	
	/**
	 * Constructs a new budgeting period form view instance.
	 * 
	 * @param container Swing component for this view
	 */
	BudgetingPeriodFormView(Container container)
	{
		forms = new HashMap<PeriodType, BudgetingPeriodFormViewModel>();
		
		layout = new CardLayout();
		view = new JPanel();
		view.setLayout(layout);
		
		typeSelection = new JComboBox(PeriodType.values());
		typeSelection.addActionListener(this);
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = GridBagConstraints.RELATIVE;
		c.gridy = 0;
		
		c.insets = new Insets(2, 0, 0, 5);
		container.add(new BoldLabel("Type"), c);
		
		c.insets = new Insets(2, 0, 0, 0);
		container.add(typeSelection, c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridwidth = 2;
		c.gridx = 0;
		c.gridy = 1;
		c.insets = new Insets(5, 0, 0, 0);
		container.add(view, c);
	}
	
	/**
	 * Registers a form component for a particular
	 * budgeting period type.
	 * 
	 * @param type      budgeting period type
	 * @param component form view component
	 * @param model     form view model
	 */
	void addForm(PeriodType type, Component component,
		BudgetingPeriodFormViewModel model)
	{
		forms.put(type, model);
		view.add(component, type.toString());
	}
	
	/**
	 * Displays the form for the specified period type.
	 * 
	 * @param type period type form to be shown
	 */
	void show(PeriodType type)
	{
		changedByApi = true;
		typeSelection.setSelectedItem(type);
	}

	@Override
    public void actionPerformed(ActionEvent arg0)
    {
		PeriodType type = (PeriodType) typeSelection.getSelectedItem();
		layout.show(view, type.toString());
		
		if ( ! changedByApi)
		{
			forms.get(type).update();
		}
		
		// Reset flag
		changedByApi = false;
    }

}
