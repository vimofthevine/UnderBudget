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
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

/**
 * Action to remove an existing condition from
 * an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class RemoveConditionAction extends AbstractAction
implements ListSelectionListener {
	
	/**
	 * Conditions table model
	 */
	private final RuleConditionsTableModel tableModel;
	
	/**
	 * Condition selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Apply-changes action model
	 */
	private final Action applyChangesAction;
	
	/**
	 * Constructs a new remove-condition action model.
	 * 
	 * @param tableModel conditions table model
	 * @param selection  condition selection model
	 * @param apply      apply-changes action
	 */
	RemoveConditionAction(RuleConditionsTableModel tableModel,
		ListSelectionModel selection, Action apply)
	{
		this.tableModel = tableModel;
		applyChangesAction = apply;
		
		selectionModel = selection;
		selectionModel.addListSelectionListener(this);
		
		putValue(NAME, "Remove Condition");
		setEnabled(false);
	}

	@Override
	public void actionPerformed(ActionEvent e)
	{
		if ( ! selectionModel.isSelectionEmpty())
		{
			int row = selectionModel.getLeadSelectionIndex();
			tableModel.removeCondition(row);
			applyChangesAction.setEnabled(true);
		}
	}

	@Override
    public void valueChanged(ListSelectionEvent event)
    {
		setEnabled( ! selectionModel.isSelectionEmpty());
    }

}
