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

package com.vimofthevine.underbudget.swing.session.source;

import java.awt.Frame;

import com.vimofthevine.underbudget.core.budget.source.BudgetSourceFactory;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * Event generated to notify the specific budget source
 * selection wizard to continue prompting the user for
 * budget source details.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SelectSourceForAction {
	
	/**
	 * Original budget source selection request
	 */
	private final SelectSource selectionRequest;
	
	/**
	 * Source type to be selected
	 */
	private final SourceType sourceType;
	
	/**
	 * User preferences
	 */
	private final UserPreferences preferences;
	
	/**
	 * Parent window
	 */
	private final Frame window;
	
	/**
	 * Constructs a new select-source action event.
	 * 
	 * @param request original select budget source request
	 * @param type    type of budget source to be selected
	 * @param prefs   user preferences
	 * @param parent  parent window
	 */
	SelectSourceForAction(SelectSource request, SourceType type,
		UserPreferences prefs, Frame parent)
	{
		selectionRequest = request;
		sourceType = type;
		preferences = prefs;
		window = parent;
	}
	
	public SourceType getSourceType()
	{
		return sourceType;
	}
	
	public boolean isOpenEvent()
	{
		return selectionRequest.isOpenRequest();
	}
	
	public Frame getParentWindow()
	{
		return window;
	}
	
	public UserPreferences getPreferences()
	{
		return preferences;
	}
	
	public void select(BudgetSourceFactory factory,
		SourceSummary summary)
	{
		selectionRequest.sourceSelected(factory, summary);
	}

}
