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

import javax.swing.TransferHandler;
import javax.swing.table.TableModel;

/**
 * Presentation model for views that display
 * a reorderable list of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRuleListModel {
	
	/**
	 * Returns the table model representing
	 * the assignment rule data.
	 * 
	 * @return assignment rules table model
	 */
	public TableModel getTableModel();
	
	/**
	 * Returns a transfer handler to execute
	 * drag-and-drop style reordering of rules
	 * within the list.
	 * 
	 * @return rule reordering transfer handler
	 */
	public TransferHandler getTransferHandler();

}
