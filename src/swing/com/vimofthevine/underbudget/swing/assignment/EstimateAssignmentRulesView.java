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

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import java.awt.Container;

import javax.swing.JButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;

/**
 * View for displaying assignment rules associated
 * with a specific estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateAssignmentRulesView {
	
	public EstimateAssignmentRulesView(Container container,
		EstimateAssignmentRulesViewModel model)
	{
		JTable table = new JTable(model.getTableModel());
		table.setSelectionModel(model.getSelectionModel());
		
		JButton addRule = new JButton(model.getAddRuleAction());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c;
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 0;
		c.gridy = 0;
		container.add(addRule, c);
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.BOTH;
		c.gridx = 0;
		c.gridy = 1;
		c.weightx = 1.0;
		c.weighty = 1.0;
		container.add(new JScrollPane(table), c);
	}

}
