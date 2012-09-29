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

package com.vimofthevine.underbudget.swing.status;

/**
 * Interface for listeners of progress actions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
interface ProgressListener {
	
	/**
	 * Starts or stops an indeterminate progress.
	 * 
	 * @param indeterminate <code>true</code> if indeterminate
	 */
	public void setIndeterminate(boolean indeterminate);
	
	/**
	 * Specifies the current progress percent value.
	 * 
	 * @param value progress percent value
	 */
	public void setValue(int value);

}
