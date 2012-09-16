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

import java.awt.Frame;
import java.net.URL;
import java.util.ArrayList;

/**
 * Basic usage tutorial.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BasicUsageTutorial {
	
	/**
	 * Constructs a new basic usage tutorial.
	 * 
	 * @param window application window
	 */
	public BasicUsageTutorial(Frame window)
	{
		ArrayList<TutorialPage> pages = new ArrayList<TutorialPage>();
		URL url;
		int i = 1;
		
		do {
			url = getClass().getResource("basic/page-" + i++ + ".html");
			if (url != null)
			{
				pages.add(new TutorialPage(url));
			}
		} while (url != null);
		
		TutorialViewModel model = new TutorialViewModel(pages);
		new TutorialDialog(window, model, "Basic Usage");
	}

}
