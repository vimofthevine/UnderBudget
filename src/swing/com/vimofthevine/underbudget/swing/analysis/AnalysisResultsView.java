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

package com.vimofthevine.underbudget.swing.analysis;

import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JScrollPane;
import javax.swing.JSeparator;

import org.jdesktop.swingx.JXTable;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View for displaying detailed analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AnalysisResultsView {
	
	public AnalysisResultsView(Container container,
		AnalysisResultsViewModel model)
	{
		container.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.PAGE_START;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = GridBagConstraints.RELATIVE;
		c.weightx = 1.0;
		
		JXTable balancesTable = new JXTable(model.getEndingBalancesTableModel());
		
		container.add(new BoldLabel("Ending Balances"), c);
		container.add(new JScrollPane(balancesTable), c);
		c.weighty = 1.0; // For last component
		container.add(new JSeparator(), c);
	}

}
