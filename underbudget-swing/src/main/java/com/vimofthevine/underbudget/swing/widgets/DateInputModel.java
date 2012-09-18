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

import java.util.Date;

import org.jdesktop.swingx.calendar.DateSelectionModel;
import org.jdesktop.swingx.calendar.DateSelectionModel.SelectionMode;

/**
 * Model for a date input field
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DateInputModel implements SelectedDateListener {
	
	/**
	 * Date selection model
	 */
	private final ObservableDateSelectionModel selection;
	
	/**
	 * Enablement model for the date field
	 */
	private final EnablementModel enabled;
	
	/**
	 * Constructs a new date input model.
	 */
	public DateInputModel()
	{
		selection = new ObservableDateSelectionModel();
		selection.setSelectionMode(SelectionMode.SINGLE_SELECTION);
		selection.addSelectedDateListener(this);
		
		enabled = new EnablementModel();
	}

	@Override
    public void dateSelectionChanged()
    {
		// Do nothing
    }
	
	/**
	 * Clears the date selection.
	 */
	public final void clear()
	{
		selection.clearSelection(false);
	}
	
	/**
	 * Updates the date selection.
	 * 
	 * @param date selected date
	 */
	public final void setDate(Date date)
	{
		selection.setSelectionInterval(date, date, false);
	}
	
	/**
	 * Returns the date selection.
	 * 
	 * @return current date selection
	 */
	public final Date getDate()
	{
		return selection.getFirstSelectionDate();
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
		this.enabled.setEnabled(enabled);
	}
	
	/**
	 * Returns the date selection model.
	 * 
	 * @return date selection model
	 */
	final DateSelectionModel getSelectionModel()
	{
		return selection;
	}
	
	/**
	 * Returns the enabled state model.
	 * 
	 * @return enabled state model
	 */
	final EnablementModel getEnablementModel()
	{
		return enabled;
	}
	
}
