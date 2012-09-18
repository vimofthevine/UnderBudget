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

import java.awt.Cursor;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.activation.ActivationDataFlavor;
import javax.activation.DataHandler;
import javax.swing.JComponent;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.TransferHandler;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.swing.assignment.events.RuleMovedEvent;

/**
 * Table row transfer handler to perform reordering
 * of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AssignmentRuleReorderHandler extends TransferHandler {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(AssignmentRuleReorderHandler.class.getName());
	
	/**
	 * Object data flavor
	 */
	private final DataFlavor localObjectFlavor;
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Table model
	 */
	private final AssignmentRulesTableModel tableModel;
	
	/**
	 * Table selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Constructs a new assignment rule reorder handler.
	 * 
	 * @param bus            event bus
	 * @param tableModel     rules table model
	 * @param selectionModel rules table selection model
	 */
	AssignmentRuleReorderHandler(EventBus bus,
		AssignmentRulesTableModel tableModel,
		ListSelectionModel selectionModel)
	{
		eventBus = bus;
		this.tableModel = tableModel;
		this.selectionModel = selectionModel;
		
		localObjectFlavor = new ActivationDataFlavor(
			Integer.class, DataFlavor.javaJVMLocalObjectMimeType,
			"Integer row");
	}
	
	@Override
	protected Transferable createTransferable(JComponent c)
	{
		int row = selectionModel.getLeadSelectionIndex();
		logger.log(Level.FINER, "Creating transferable of row " + row);
		return new DataHandler(new Integer(row), localObjectFlavor.getMimeType());
	}
	
	@Override
	public boolean canImport(TransferSupport info)
	{
		return (info.getComponent() instanceof JTable)
			&& info.isDrop()
			&& info.isDataFlavorSupported(localObjectFlavor);
	}
	
	@Override
	public int getSourceActions(JComponent c)
	{
		return TransferHandler.MOVE;
	}
	
	@Override
	public boolean importData(TransferSupport info)
	{
		JTable target = (JTable) info.getComponent();
		JTable.DropLocation dropLocation = (JTable.DropLocation) info.getDropLocation();
		int index = dropLocation.getRow();
		int max = tableModel.getRowCount();
		
		if (index < 0 || index > max)
			index = max;
		
		target.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
		
		try
		{
			Integer fromRow = (Integer) info.getTransferable().getTransferData(localObjectFlavor);
			
			if (fromRow != -1 && fromRow != index)
			{
				tableModel.reorder(fromRow, index);
				eventBus.post(new RuleMovedEvent(fromRow, index));
				
				if (index > fromRow)
				{
					index--;
				}
				
				selectionModel.setSelectionInterval(index, index);
			}
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Error reordering assignment rules", e);
		}
		
		return false;
	}
	
	@Override
	protected void exportDone(JComponent c, Transferable t, int action)
	{
		if (action == TransferHandler.MOVE)
		{
			// Set cursor to default
		}
	}

}
