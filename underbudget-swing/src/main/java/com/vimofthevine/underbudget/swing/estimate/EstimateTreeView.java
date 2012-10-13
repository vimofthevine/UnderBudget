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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTable;
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
		final JXTreeTable treeTable = new JXTreeTable(model.getTreeTableModel());
		treeTable.setDefaultRenderer(JProgressBar.class, this);
		treeTable.expandAll();
		treeTable.setRowHeight(treeTable.getRowHeight() + 2);
		
		model.getTreeTableModel().addRefreshListener(new RefreshListener() {
			public void refresh()
			{
				treeTable.repaint();
			}
		});
		
		model.setTreeSelectionModel(treeTable.getTreeSelectionModel());
		
		// Remove icons
		treeTable.setClosedIcon(null);
		treeTable.setOpenIcon(null);
		treeTable.setLeafIcon(null);
		
		container.setLayout(new BorderLayout());
		container.add(new JScrollPane(treeTable), BorderLayout.CENTER);
		
		createPopupMenu(model, treeTable);
	}

	/**
     * @see javax.swing.table.TableCellRenderer#getTableCellRendererComponent(javax.swing.JTable, java.lang.Object, boolean, boolean, int, int)
     */
    @Override
    public Component getTableCellRendererComponent(JTable table,
    	Object value, boolean isSelected, boolean hasFocus, int row, int column)
    {
    	if (value instanceof JProgressBar)
    	{
    		JProgressBar progress = (JProgressBar) value;
    		progress.setBorderPainted(false);
    		progress.setBackground(isSelected ?
    			UIManager.getColor("Table.selectionBackground") :
    			UIManager.getColor("Table.background"));
    		return progress;
    	}
    	else
    		return null;
    }
    
    private void createPopupMenu(EstimateTreeViewModel model, Component component)
    {
    	final JPopupMenu popup = new JPopupMenu();
    	popup.add(new JMenuItem(model.getAddChildToRootAction()));
    	
    	component.addMouseListener(new MouseAdapter() {
			@Override
            public void mousePressed(MouseEvent event)
            {
				showPopup(event);
            }

			@Override
            public void mouseReleased(MouseEvent event)
            {
				showPopup(event);
            }
    		
			private void showPopup(MouseEvent event)
			{
				if (event.isPopupTrigger())
				{
					popup.show(event.getComponent(),
						event.getX(), event.getY());
				}
			}
    	});
    }

}
