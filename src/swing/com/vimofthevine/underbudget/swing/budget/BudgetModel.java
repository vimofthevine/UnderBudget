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

package com.vimofthevine.underbudget.swing.budget;

import javax.swing.Action;
import javax.swing.ComboBoxModel;
import javax.swing.text.Document;
import javax.swing.text.LayoutQueue;

/**
 * Presentation model for views that display
 * modifiable details about a budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetModel {

	/**
	 * Returns a document representing
	 * the budget's name.
	 * 
	 * @return budget name document
	 */
	public Document getNameDocument();
	
	/**
	 * Returns a document representing
	 * the budget's initial balance.
	 * 
	 * @return budget name document
	 */
	public Document getInitialBalanceDocument();
	
	/**
	 * Returns a document representing
	 * the budget's period.
	 * 
	 * @return budget period document
	 */
	public Document getPeriodDocument();
	
	/**
	 * Returns a combo box model representing
	 * the budgeting period type.
	 * 
	 * @return budgeting period type combo box model
	 */
	public ComboBoxModel getPeriodTypeModel();
	
	/**
	 * Returns a layout manager for the
	 * individual budgeting period forms.
	 * 
	 * @return budgeting period forms layout manager
	 */
	public LayoutQueue getPeriodTypeLayout();
	
	/**
	 * Returns an action for closing the
	 * budget modification dialog.
	 * 
	 * @return close dialog action
	 */
	public Action getCloseAction();
	
}
