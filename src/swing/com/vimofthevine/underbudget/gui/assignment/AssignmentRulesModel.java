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

package com.vimofthevine.underbudget.gui.assignment;

import javax.swing.Action;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableModel;

/**
 * Presentation model for views that display a
 * list of rules associated with an individual
 * estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRulesModel {

	/**
	 * Returns the table model representing
	 * the associated assignment rule data.
	 * 
	 * @return associated rules table model
	 */
	public TableModel getTableModel();
	
	/**
	 * Returns a listener to respond to the
	 * selecting of an assignment rule in the list.
	 * 
	 * @return rule selection listener
	 */
	public ListSelectionListener getSelectionListener();
	
	/**
	 * Returns an action for creating a new
	 * rule associated with the current estimate.
	 * 
	 * @return create rule action
	 */
	public Action getAddRuleAction();
	
}
