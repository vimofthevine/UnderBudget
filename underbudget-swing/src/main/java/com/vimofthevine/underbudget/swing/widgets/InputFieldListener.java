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

package com.vimofthevine.underbudget.swing.widgets;

/**
 * Interface for listeners to be notified when
 * an input field has been modified, or updated,
 * by the user.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface InputFieldListener {
	
	/**
	 * Notifies the listener that the input field
	 * has been modified by the user.
	 */
	public void fieldChanged();

}