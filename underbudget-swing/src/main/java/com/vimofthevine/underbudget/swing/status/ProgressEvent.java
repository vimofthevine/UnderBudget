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
 * Event generated to update the progress bar within
 * the status bar.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ProgressEvent {

	/**
	 * Progress value
	 */
	private final int value;
	
	/**
	 * Whether to start or stop an indeterminate progress
	 */
	private final boolean indeterminate;
	
	/**
	 * Constructs a new progress event to update
	 * the progress bar to the given percent value.
	 * <p>
	 * If the given value is less than 0, it
	 * will be treated as 0. If the given value
	 * is greater than 100, it will be treated as 100.
	 * 
	 * @param value
	 */
	public ProgressEvent(int value)
	{
		this.value = value;
		indeterminate = false;
	}
	
	/**
	 * Constructs a new progress event to start
	 * or stop an indeterminate progress.
	 * 
	 * @param indeterminate <code>true</code> to begin an indeterminate
	 *                      progress, else <code>false</code> to stop
	 *                      an indeterminate progress
	 */
	public ProgressEvent(boolean indeterminate)
	{
		this.value = 0;
		this.indeterminate = indeterminate;
	}
	
	/**
	 * Returns the new progress value.
	 * 
	 * @return progress percent value
	 */
	public int getValue()
	{
		return value;
	}
	
	/**
	 * Checks if the progress is indeterminate.
	 * 
	 * @return <code>true</code> if indeterminate
	 */
	public boolean isIndeterminate()
	{
		return indeterminate;
	}
	
	@Override
	public String toString()
	{
		return "Progress event {" + value + "," + indeterminate + "}";
	}

}
