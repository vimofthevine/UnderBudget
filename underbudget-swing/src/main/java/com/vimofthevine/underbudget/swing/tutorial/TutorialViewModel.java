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

package com.vimofthevine.underbudget.swing.tutorial;

import java.awt.event.ActionEvent;
import java.util.List;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.SwingUtilities;

import com.vimofthevine.underbudget.swing.window.WindowStateModel;

/**
 * View model for a step-by-step, multi-page
 * tutorial dialog.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class TutorialViewModel {
	
	/**
	 * Tutorial content model
	 */
	private final TutorialContentModel content;
	
	/**
	 * Window state model
	 */
	private final WindowStateModel window;

	/**
	 * Previous-page action model
	 */
	private final Action backAction;
	
	/**
	 * Next-page action model
	 */
	private final Action nextAction;
	
	/**
	 * Skip/close tutorial action model
	 */
	private final Action skipAction;
	
	/**
	 * Tutorial pages
	 */
	private final List<TutorialPage> pages;
	
	/**
	 * Current page index
	 */
	private int page;
	
	/**
	 * Constructs a new tutorial view model.
	 * 
	 * @param tutorialPages tutorial pages
	 */
	TutorialViewModel(List<TutorialPage> tutorialPages)
	{
		pages = tutorialPages;
		page = 0;
		
		content = new TutorialContentModel();
		window = new WindowStateModel();
		
		backAction = new AbstractAction() {
			{
				putValue(NAME, "Previous");
			}
			
			@Override
			public void actionPerformed(ActionEvent event)
			{
				if (page > 0)
				{
					page = page - 1;
					update();
				}
			}
		};
		
		nextAction = new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				if (page < (pages.size() - 1))
				{
					page = page + 1;
					update();
				}
				else if (page == (pages.size() - 1))
				{
					window.close();
				}
			}
		};
		
		skipAction = new AbstractAction() {
			{
				putValue(NAME, "Skip");
			}
			
			@Override
			public void actionPerformed(ActionEvent event)
			{
				window.close();
			}
		};
		
		update();
	}
	
	/**
	 * Updates the view with the proper current page's
	 * attributes.
	 */
	private void update()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				backAction.setEnabled(page > 0);
				nextAction.setEnabled(page < pages.size());
				nextAction.putValue(Action.NAME, (page == (pages.size()-1))
					? "Got it, wish me luck!" : "Next");
				
				if (page >=0 && page < pages.size())
				{
					content.setContent(pages.get(page).getUrl());
				}
			}
		});
	}
	
	/**
	 * Returns the content model
	 * 
	 * @return content model
	 */
	TutorialContentModel getContent()
	{
		return content;
	}
	
	/**
	 * Returns the window state model.
	 * 
	 * @return window state model
	 */
	WindowStateModel getWindowState()
	{
		return window;
	}
	
	/**
	 * Returns the previous-page action model.
	 * 
	 * @return previous-page action model
	 */
	Action getBackAction()
	{
		return backAction;
	}
	
	/**
	 * Returns the next-page action model.
	 * 
	 * @return next-page action model
	 */
	Action getNextAction()
	{
		return nextAction;
	}
	
	/**
	 * Returns the skip/close action model.
	 * 
	 * @return skip/close action model
	 */
	Action getSkipAction()
	{
		return skipAction;
	}

}
