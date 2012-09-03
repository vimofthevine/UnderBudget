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

import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;

import javax.swing.SwingUtilities;

/**
 * A view responsible for switching between requested
 * session content views.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class SessionContentView implements DisplayedContentListener {
	
	/**
	 * Content view
	 */
	private final Container view;
	
	/**
	 * Card layout
	 */
	private final CardLayout layout;
	
	/**
	 * Displayed content model
	 */
	private final DisplayedContentModel contentModel;
	
	/**
	 * Constructs a new session content view.
	 * 
	 * @param container Swing component for this view
	 * @param model     presentation model
	 */
	SessionContentView(Container container,
		SessionContentViewModel model)
	{
		view = container;
		layout = new CardLayout();
		view.setLayout(layout);
		
		contentModel = model.getContentModel();
		contentModel.addDisplayedContentListener(this);
	}
	
	/**
	 * Adds a content view component.
	 * 
	 * @param component content view component
	 * @param content   session content type
	 */
	void add(final Component component,
		final SessionContent content)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				view.add(component, content.toString());
				
				if (content.equals(contentModel.getCurrentContent()))
				{
					layout.show(view,  content.toString());
				}
			}
		});
	}

	@Override
    public void display(final SessionContent content)
    {
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				layout.show(view, content.toString());
			}
		});
    }

}
