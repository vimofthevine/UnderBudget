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

package com.vimofthevine.underbudget.swing.summary;

import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTextField;

import org.jdesktop.swingx.JXTable;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View for displaying detailed analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AnalysisResultsView {
	
	/**
	 * Constructs a new analysis results view instance.
	 * 
	 * @param container Swing component for this view
	 * @param model     analysis results view model
	 */
	public AnalysisResultsView(Container container,
		AnalysisResultsViewModel model)
	{
		JTextField name = new JTextField();
		name.setDocument(model.getBudgetNameModel());
		name.setEditable(false);
		
		JTextField period = new JTextField();
		period.setDocument(model.getBudgetingPeriodModel());
		period.setEditable(false);
		
		JXTable balancesTable = new JXTable(model.getEndingBalancesTableModel());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridy = GridBagConstraints.RELATIVE;
		
		c.gridx = 0;
		c.insets = new Insets(5, 2, 0, 5);
		c.weightx = 0.0;
		container.add(new BoldLabel("Budget Name"), c);
		
		c.gridx = 1;
		c.insets = new Insets(5, 0, 0, 2);
		c.weightx = 1.0;
		container.add(name, c);
		
		c.gridx = 0;
		c.insets = new Insets(5, 2, 0, 5);
		c.weightx = 0.0;
		container.add(new BoldLabel("Budgeting Period"), c);
		
		c.gridx = 1;
		c.insets = new Insets(5, 0, 0, 2);
		c.weightx = 1.0;
		container.add(period, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.PAGE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridwidth = 2;
		c.gridx = 0;
		c.gridy = GridBagConstraints.RELATIVE;
		c.insets = new Insets(5, 2, 0, 2);
		c.weightx = 1.0;
		
		container.add(new BoldLabel("Ending Balances"), c);
		container.add(new JScrollPane(balancesTable), c);
		c.weighty = 1.0; // For last component
		container.add(new JSeparator(), c);
	}

}
