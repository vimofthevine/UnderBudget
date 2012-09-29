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
 * Event generated to update the status bar with
 * a given message.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StatusMessageEvent {
	
	/**
	 * Status message
	 */
	private final String message;
	
	/**
	 * Optional duration (in ms) for
	 * the message to be displayed
	 */
	private final int duration;
	
	/**
	 * Constructs a new status message event.
	 * The message will be displayed in the
	 * status bar until a new message is displayed.
	 * 
	 * @param message status message
	 */
	public StatusMessageEvent(String message)
	{
		this(message, -1);
	}
	
	/**
	 * Constructs a new status message event.
	 * The message will be displayed in the
	 * status bar for the specified duration
	 * then cleared.
	 * 
	 * @param message  status message
	 * @param duration message display duration in milliseconds
	 */
	public StatusMessageEvent(String message, int duration)
	{
		this.message = message;
		this.duration = duration;
	}
	
	/**
	 * Returns the status message to be displayed.
	 * 
	 * @return status message
	 */
	public String getMessage()
	{
		return message;
	}
	
	/**
	 * Returns the display duration of the message.
	 * 
	 * @return message display duration in milliseconds
	 */
	public int getDuration()
	{
		return duration;
	}
	
	@Override
	public String toString()
	{
		return "Status message event {" + message + "," + duration + "}";
	}

}
