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

package com.vimofthevine.underbudget.gui.content;

import java.util.HashMap;
import java.util.Map;

import com.google.common.eventbus.Subscribe;

/**
 * This feels a little hokey. Come back and revisit it.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ContentViewModel {

	/**
	 * Content view
	 */
	private final ContentView content;
	
	/**
	 * Known content dialogs
	 */
	private final Map<String, ContentDialog> dialogs;
	
	/**
	 * Constructs a new content view presentation model.
	 * 
	 * @param content content view
	 * @param dialogs content display dialogs
	 */
	public ContentViewModel(ContentView content,
		Map<String,ContentDialog> dialogs)
	{
		this.content = content;
		this.dialogs = (dialogs == null)
			? new HashMap<String, ContentDialog>() : dialogs;
	}
	
	@Subscribe
	public void displayContent(ContentDisplayEvent event)
	{
		ContentDisplay name = event.getView();
		switch (event.getView())
		{
			case ABOUT:
				if (dialogs.containsKey(name))
				{
					dialogs.get(name).display();
				}
				break;
			case ASSIGNMENT_RULES:
				content.showDisplay(name);
				break;
			case BALANCE_IMPACT:
				content.showDisplay(name);
				break;
			case EDIT_BUDGET:
				if (dialogs.containsKey(name))
				{
					dialogs.get(name).display();
				}
				break;
			case ESTIMATE_PROGRESS:
				content.showDisplay(name);
				break;
			case IMPORTED_TRANSACTIONS:
				content.showDisplay(name);
				break;
			case PREFERENCES:
				if (dialogs.containsKey(name))
				{
					dialogs.get(name).display();
				}
				break;
		}
	}
	
}
