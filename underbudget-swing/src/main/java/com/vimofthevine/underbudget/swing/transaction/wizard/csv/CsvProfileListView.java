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

package com.vimofthevine.underbudget.swing.transaction.wizard.csv;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JScrollPane;

/**
 * A view for selecting a CSV profile.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class CsvProfileListView {
	
	/**
	 * Constructs a new CSV profile list view instance.
	 * 
	 * @param container Swing component for this view
	 * @param model     CSV profile list view model
	 */
	CsvProfileListView(Container container,
		CsvProfileListViewModel model)
	{
		JList list = new JList(model.getListModel());
		list.setSelectionModel(model.getSelectionModel());
		JScrollPane scroll = new JScrollPane(list);
		scroll.setPreferredSize(new Dimension(400, 150));
		
		JButton select = new JButton(model.getSelectAction());
		JButton cancel = new JButton(model.getCancelAction());
		
		container.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		c.fill = GridBagConstraints.BOTH;
		c.gridwidth = 2;
		c.gridx = 0;
		c.gridy = 0;
		c.weightx = 1.0;
		c.weighty = 1.0;
		container.add(scroll, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_START;
		c.gridx = 0;
		c.gridy = 1;
		c.insets = new Insets(5, 5, 5, 5);
		c.weightx = 1.0;
		container.add(cancel, c);
		
		c = new GridBagConstraints();
		c.gridx = 1;
		c.gridy = 1;
		c.insets = new Insets(5, 0, 5, 5);
		container.add(select, c);
	}

}
