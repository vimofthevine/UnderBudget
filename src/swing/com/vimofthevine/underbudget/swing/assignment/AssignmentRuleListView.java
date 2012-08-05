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

import java.awt.BorderLayout;
import java.awt.Container;

import javax.swing.DropMode;
import javax.swing.JScrollPane;
import javax.swing.JTable;

/**
 * Table to display the prioritized list of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignmentRuleListView {
	
	/**
	 * Constructs a new assignment rule list view.
	 * 
	 * @param container Swing component for the view
	 * @param model     rule list view model
	 */
	public AssignmentRuleListView(Container container,
		AssignmentRuleListViewModel model)
	{
		JTable table = new JTable(model.getTableModel());
		table.setSelectionModel(model.getSelectionModel());
		
		table.setDragEnabled(true);
		table.setDropMode(DropMode.INSERT_ROWS);
		table.setTransferHandler(model.getTransferHandler());
		
		container.setLayout(new BorderLayout());
		container.add(new JScrollPane(table), BorderLayout.CENTER);
	}

}
