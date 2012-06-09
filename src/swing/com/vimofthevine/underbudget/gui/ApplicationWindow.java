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

package com.vimofthevine.underbudget.gui;

/**
 * Primary application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface ApplicationWindow {
	
	/**
	 * Specifies the title to be displayed on the
	 * application window chrome.
	 * 
	 * @param title application window title
	 */
	public void setWindowTitle(final String title);
	
	/**
	 * Specifies the size of the application window.
	 * 
	 * @param width  application window width
	 * @param height application window height
	 */
	public void setWindowSize(final int width, final int height);
	
	/**
	 * Displays the application window.
	 */
	public void display();
	
	/**
	 * Closes and disposes of the application window.
	 */
	public void dispose();
	
}
