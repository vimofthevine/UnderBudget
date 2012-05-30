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

package com.vimofthevine.underbudget.core.progress;

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;


/**
 * Custom input stream to track progress of the reading
 * of an input stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ProgressMonitorInputStream extends FilterInputStream {
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Total number of bytes expected to be read
	 */
	private int max = 100;

	/**
	 * Constructs an object to monitor the progress of an input stream
	 * 
	 * @param progress task progress updater
	 * @param in       input stream to be monitored
	 */
	public ProgressMonitorInputStream(TaskProgress progress, InputStream in)
    {
	    super(in);
	    
	    this.progress = progress;
	    
	    try
	    {
	    	max = in.available();
	    }
	    catch (IOException ioe) { }
    }
	
	@Override
	public int read() throws IOException
	{
		int read = super.read();
		
		updateProgress(read);
		
		return read;
	}
	
	@Override
	public int read(byte[] data) throws IOException
	{
		int read = super.read(data);
		
		updateProgress(read);
		
		return read;
	}
	
	@Override
	public int read(byte[] data, int offset, int length) throws IOException
	{
		int read = super.read(data, offset, length);
		
		updateProgress(read);
		
		return read;
	}
	
	@Override
	public long skip(long length) throws IOException
	{
		long read = super.skip(length);
		
		updateProgress((int) read);
		
		return read;
	}
	
	/**
	 * Updates the current task progress according
	 * to the number of bytes read
	 * 
	 * @param read number of bytes read
	 */
	protected void updateProgress(int read)
	{
		if (read > 0)
		{
			float percentage = ((float) read / (float) max) * 100;
			progress.increasePercentCompleteBy(percentage);
		}
	}

}
