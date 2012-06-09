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

/**
 * Event generated when the user requests to view
 * a specific application view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ContentDisplayEvent {
	
	/**
	 * Requested view to be displayed
	 */
	private final ContentDisplay view;
	
	/**
	 * Constructs a new view display event
	 * for the given view.
	 * 
	 * @param view requested view
	 */
	public ContentDisplayEvent(ContentDisplay view)
	{
		this.view = view;
	}
	
	/**
	 * Returns the view that has been
	 * requested to be displayed.
	 * 
	 * @return requested view to display
	 */
	public ContentDisplay getView()
	{
		return view;
	}
	
}
