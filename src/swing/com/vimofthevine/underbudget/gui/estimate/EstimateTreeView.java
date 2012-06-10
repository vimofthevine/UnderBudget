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

package com.vimofthevine.underbudget.gui.estimate;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;

import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.table.TableCellRenderer;

import org.jdesktop.swingx.JXTreeTable;

/**
 * A hierarchical view of all defined estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateTreeView implements TableCellRenderer {
	
	/**
	 * Constructs a new hierarchical estimate view.
	 * 
	 * @param container Swing component for the view
	 * @param model     estimate tree view model
	 */
	public EstimateTreeView(Container container,
		EstimateTreeViewModel model)
	{
		JXTreeTable treeTable = new JXTreeTable(model.getTreeTableModel());
		treeTable.setDefaultRenderer(EstimateProgressBar.class, this);
		treeTable.expandAll();
		treeTable.setRowHeight(treeTable.getRowHeight() + 2);
		
		model.setTreeSelectionModel(treeTable.getTreeSelectionModel());
		treeTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		treeTable.addTreeSelectionListener(model.getSelectionListener());
		
		// Remove icons
		treeTable.setClosedIcon(null);
		treeTable.setOpenIcon(null);
		treeTable.setLeafIcon(null);
		
		container.setLayout(new BorderLayout());
		container.add(new JScrollPane(treeTable), BorderLayout.CENTER);
	}

	/**
     * @see javax.swing.table.TableCellRenderer#getTableCellRendererComponent(javax.swing.JTable, java.lang.Object, boolean, boolean, int, int)
     */
    @Override
    public Component getTableCellRendererComponent(JTable table,
    	Object value, boolean isSelected, boolean hasFocus, int row, int column)
    {
    	if (value instanceof EstimateProgressBar)
    	{
    		JProgressBar progress = ((EstimateProgressBar) value).getProgressBar();
    		progress.setBorderPainted(false);
    		progress.setBackground(isSelected ?
    			UIManager.getColor("Table.selectionBackground") :
    			UIManager.getColor("Table.background"));
    		return progress;
    	}
    	else
    		return null;
    }

}
