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

package com.vimofthevine.underbudget.swing.session.content;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.session.events.SessionContentEvent;

/**
 * Presentation model for a view responsible for
 * switching between requested session content views.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class SessionContentViewModel {
	
	/**
	 * Displayed content model
	 */
	private final DisplayedContentModel contentModel;
	
	/**
	 * Constructs a new session content view presentation
	 * model.
	 * 
	 * @param bus session event bus
	 */
	SessionContentViewModel(EventBus bus)
	{
		bus.register(this);
		
		contentModel = new DisplayedContentModel();
	}
	
	/**
	 * Returns the displayed content model.
	 * 
	 * @return displayed content model
	 */
	DisplayedContentModel getContentModel()
	{
		return contentModel;
	}

	/**
	 * Updates the displayed content model with
	 * the selected session content, if the
	 * requested content is not displayed as a
	 * popup dialog.
	 * 
	 * @param event session content event
	 */
	@Subscribe
	public void display(SessionContentEvent event)
	{
		switch(event.getContent())
		{
			case ANALYSIS_SUMMARY:
			case ASSIGNMENT_RULES:
			case BALANCE_IMPACT:
			case ESTIMATE_PROGRESS:
			case IMPORTED_TRANSACTIONS:
				
				contentModel.setContent(event.getContent());
				break;
				
			case BUDGET:
				break;
				
			default:
				// Do nothing (content is not displayed in this view)
		}
	}
	
}
