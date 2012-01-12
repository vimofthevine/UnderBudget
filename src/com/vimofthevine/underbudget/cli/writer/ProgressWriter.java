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

package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

import com.vimofthevine.underbudget.util.task.TaskProgressListener;

/**
 * Writes progress updates to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ProgressWriter implements TaskProgressListener {
	
	/**
	 * Progress status message
	 */
	protected String message;
	
	/**
	 * Print stream used for writing progress
	 */
	protected PrintStream stream;
	
	/**
	 * Flag to indicate that the progress-complete message
	 * has already been printed
	 */
	protected boolean complete = false;
	
	/**
	 * Class constructor
	 * 
	 * @param stream print stream to which to write
	 */
	public ProgressWriter(String msg, PrintStream stream)
	{
		message = msg;
		this.stream = stream;
	}

	@Override
    public void progressUpdate(int value)
    {
		stream.print(message + " " + value + "%" + "\r");
		
		// Only print "done" once
		if (value == 100 && ! complete)
		{
			complete = true;
			
			stream.println();
			stream.println("Done");
		}
    }

}
