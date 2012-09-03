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
 * Event generated to update the status bar in
 * the application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StatusEvent {
	
	/**
	 * New status message
	 */
	public final String message;
	
	/**
	 * New progress value
	 */
	public final int progress;
	
	/**
	 * Constructs a new status event.
	 * 
	 * @param message new status message
	 */
	public StatusEvent(String message)
	{
		this(message, -1);
	}
	
	/**
	 * Constructs a new status event.
	 * 
	 * @param progress new progress value
	 */
	public StatusEvent(int progress)
	{
		this(null, progress);
	}
	
	/**
	 * Constructs a new status event.
	 * 
	 * @param message  new status message
	 * @param progress new progress value
	 */
	public StatusEvent(String message, int progress)
	{
		this.message = message;
		this.progress = progress;
	}
	
	/**
	 * Returns the new status message.
	 * 
	 * @return new status message
	 */
	public String getMessage()
	{
		return message;
	}
	
	/**
	 * Returns the new progress value.
	 * 
	 * @return new progress value
	 */
	public int getProgress()
	{
		return progress;
	}

}
