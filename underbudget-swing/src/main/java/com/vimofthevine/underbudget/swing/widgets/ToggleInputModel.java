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
import javax.swing.JToggleButton.ToggleButtonModel;

/**
 * Model for a toggle (checkbox/radio) input field.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ToggleInputModel implements SelectionToggleListener {
	
	/**
	 * Toggle button model for the toggle field
	 */
	private final ObservableToggleButtonModel model;
	
	/**
	 * Action model for the toggle field
	 */
	private final Action action;
	
	/**
	 * Constructs a new toggle input model.
	 */
	public ToggleInputModel()
	{
		model = new ObservableToggleButtonModel();
		model.addSelectionToggleListener(this);
		
		action = new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				// Do nothing
			}
		};
	}
	
	/**
	 * Updates the toggle selection state.
	 * 
	 * @param selected toggle selection
	 */
	public final void setSelected(boolean selected)
	{
		model.setSelected(selected, false);
	}
	
	/**
	 * Returns the current toggle selection.
	 * 
	 * @return current toggle selection
	 */
	public final boolean isSelected()
	{
		return model.isSelected();
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
    public void selectionToggled(boolean selected)
    {
		// Do nothing
    }
	
	/**
	 * Returns the toggle button model.
	 * 
	 * @return toggle button model
	 */
	final ToggleButtonModel getModel()
	{
		return model;
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
