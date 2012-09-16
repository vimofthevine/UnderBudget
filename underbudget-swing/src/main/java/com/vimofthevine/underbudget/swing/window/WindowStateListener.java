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

package com.vimofthevine.underbudget.swing.window;

/**
 * Interface for listeners to be notified of
 * desired changes to a window's state.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface WindowStateListener {
	
	/**
	 * Notifies the listener of the desire
	 * to close the window.
	 */
	public void closeWindow();

}
