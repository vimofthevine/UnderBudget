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

package com.vimofthevine.underbudget.swing.budget.period;

import javax.swing.ComboBoxModel;
import javax.swing.SpinnerModel;
import javax.swing.text.Document;

/**
 * Presentation model for views that display
 * an input form for defining a literal month
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface MonthFormViewModel
extends BudgetingPeriodFormViewModel {
	
	/**
	 * Returns a document representing the
	 * summary of the budgeting period.
	 * 
	 * @return budgeting period summary document
	 */
	Document getSummaryDocument();

	/**
	 * Returns the combo box model for the
	 * literal month selection.
	 * 
	 * @return month combo box model
	 */
	ComboBoxModel getMonthModel();
	
	/**
	 * Returns the spinner model for the
	 * literal month's year selection.
	 * 
	 * @return year spinner model
	 */
	SpinnerModel getYearModel();
	
}
