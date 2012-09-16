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

import java.net.URL;

/**
 * An individual tutorial page.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class TutorialPage {
	
	private final URL url;
	
	/**
	 * Constructs a new tutorial page.
	 * 
	 * @param contentURL content URL
	 */
	TutorialPage(URL contentUrlL)
	{
		this.url = contentUrlL;
	}
	
	/**
	 * Returns the URL for this pages's content.
	 * 
	 * @return content URL
	 */
	URL getUrl()
	{
		return url;
	}

}
