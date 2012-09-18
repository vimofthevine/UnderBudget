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

/**
 * Interface for listeners to receive notice that
 * the displayed content has changed.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
interface DisplayedContentListener {
	
	/**
	 * Notifies the listener of the content
	 * to be displayed.
	 * 
	 * @param content content to be displayed
	 */
	public void display(SessionContent content);

}
