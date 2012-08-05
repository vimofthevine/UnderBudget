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

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;

/**
 * Action to add a new condition to an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AddConditionAction extends AbstractAction {
	
	/**
	 * Conditions table model
	 */
	private final RuleConditionsTableModel tableModel;
	
	/**
	 * Apply-changes action model
	 */
	private final Action applyChangesAction;
	
	/**
	 * Constructs a new add-condition action model.
	 * 
	 * @param tableModel conditions table model
	 * @param apply      apply-changes action
	 */
	AddConditionAction(RuleConditionsTableModel tableModel,
		Action apply)
	{
		this.tableModel = tableModel;
		applyChangesAction = apply;
		
		putValue(NAME, "Add Condition");
	}

	@Override
	public void actionPerformed(ActionEvent arg0)
	{
		tableModel.addCondition(new UserCondition());
		applyChangesAction.setEnabled(true);
	}

}
