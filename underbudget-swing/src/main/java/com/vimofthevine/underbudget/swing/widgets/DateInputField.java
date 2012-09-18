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

import org.jdesktop.swingx.JXDatePicker;

/**
 * Customized date picker using a date
 * input model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DateInputField extends JXDatePicker {
	
	/**
	 * Constructs a new date input field for
	 * the given date input model.
	 * 
	 * @param model date input model
	 */
	public DateInputField(DateInputModel model)
	{
		super();
		getMonthView().setSelectionModel(model.getSelectionModel());
		
		model.getEnablementModel().addListener(new EnablementListener() {
			@Override
            public void enable(boolean enabled)
            {
				setEnabled(enabled);
            }
		});
	}

}
