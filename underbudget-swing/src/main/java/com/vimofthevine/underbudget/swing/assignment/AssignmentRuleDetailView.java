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

package com.vimofthevine.underbudget.swing.assignment;

import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JScrollPane;
import javax.swing.JTable;

import org.jdesktop.swingx.autocomplete.ComboBoxCellEditor;

import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.TransactionField;

/**
 * View for displaying modifiable details about
 * a specific assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignmentRuleDetailView {
	
	/**
	 * Constructs a new assignment rule detail view.
	 * 
	 * @param container Swing component for this view
	 * @param model     assignment rule detail view model
	 */
	public AssignmentRuleDetailView(Container container,
		AssignmentRuleDetailViewModel model)
	{
		JTable table = new JTable(model.getConditionsTableModel());
		table.setSelectionModel(model.getConditionSelectionModel());
		table.setDefaultEditor(TransactionField.class,
			new ComboBoxCellEditor(new JComboBox(TransactionField.values())));
		table.setDefaultEditor(EvaluationOperator.class,
			new ComboBoxCellEditor(new JComboBox(EvaluationOperator.values())));
		
		JButton addCondition = new JButton(model.getAddConditionAction());
		JButton removeCondition = new JButton(model.getRemoveConditionAction());
		JButton applyChanges = new JButton(model.getApplyChangesAction());
		JButton deleteRule = new JButton(model.getDeleteAction());
		JButton cloneRule = new JButton(model.getCloneAction());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c;
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.BOTH;
		c.gridheight = 5;
		c.gridx = 0;
		c.gridy = 0;
		c.weightx = 1.0;
		c.weighty = 1.0;
		container.add(new JScrollPane(table), c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 0;
		container.add(addCondition, c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 1;
		container.add(removeCondition, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.PAGE_END;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 2;
		c.weighty = 1.0;
		container.add(applyChanges, c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 3;
		container.add(cloneRule, c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 4;
		container.add(deleteRule, c);
	}

}
