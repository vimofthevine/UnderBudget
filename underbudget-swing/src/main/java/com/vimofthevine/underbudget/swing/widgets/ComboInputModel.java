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

package com.vimofthevine.underbudget.swing.widgets;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ComboBoxModel;

/**
 * Model for a dropdown selection input field.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ComboInputModel implements ItemSelectionListener {
	
	/**
	 * Combo box model
	 */
	private final ObservableComboBoxModel comboBoxModel;
	
	/**
	 * Action model for the combo field
	 */
	private final Action action;
	
	/**
	 * Constructs a new combo input model.
	 * 
	 * @param items selectable items
	 */
	public ComboInputModel(Object[] items)
	{
		comboBoxModel = new ObservableComboBoxModel(items);
		comboBoxModel.addItemSelectionListener(this);
		
		action = new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				// Do nothing
			}
		};
	}
	
	/**
	 * Updates the selected item in the combo box.
	 * 
	 * @param item new selected item
	 */
	public final void setSelectedItem(Object item)
	{
		comboBoxModel.setSelectedItem(item, false);
	}
	
	/**
	 * Returns the currently selected item.
	 * 
	 * @return currently selected item
	 */
	public final Object getSelectedItem()
	{
		return comboBoxModel.getSelectedItem();
	}
	
	/**
	 * Specifies whether the input field is
	 * enabled or disabled.
	 * 
	 * @param enabled <code>true</code> if the
	 *                field is enabled
	 */
	public final void setEnabled(boolean enabled)
	{
		action.setEnabled(enabled);
	}

	@Override
    public void itemSelected(Object item)
    {
		// Do nothing
    }

	/**
	 * Returns the combo box model.
	 * 
	 * @return combo box model
	 */
	final ComboBoxModel getComboBoxModel()
	{
		return comboBoxModel;
	}
	
	/**
	 * Returns the input action model.
	 * 
	 * @return input action model
	 */
	final Action getAction()
	{
		return action;
	}
	
}
