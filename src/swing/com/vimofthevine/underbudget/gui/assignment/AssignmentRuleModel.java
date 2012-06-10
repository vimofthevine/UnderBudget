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
import javax.swing.table.TableModel;

/**
 * Presentation model for views that display
 * modifiable details about an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRuleModel {

	/**
	 * Returns the table model representing
	 * the rule conditions data.
	 * 
	 * @return rule conditions table model
	 */
	public TableModel getConditionsTableModel();
	
	/**
	 * Returns an action for adding a new
	 * rule condition.
	 * 
	 * @return create condition action
	 */
	public Action getAddConditionAction();
	
	/**
	 * Returns an action for removing a new
	 * rule condition.
	 * 
	 * @return removing condition action
	 */
	public Action getRemoveConditionAction();
	
	/**
	 * Returns an action for applying the
	 * changes made for the rule conditions.
	 * 
	 * @return apply conditions action
	 */
	public Action getApplyConditionsAction();
	
	/**
	 * Returns an action for deleting the
	 * assignment rule.
	 * 
	 * @return delete rule action
	 */
	public Action getDeleteAction();
	
	/**
	 * Returns an action for cloning the
	 * assignment rule
	 * 
	 * @return clone rule action
	 */
	public Action getCloneAction();
	
}
