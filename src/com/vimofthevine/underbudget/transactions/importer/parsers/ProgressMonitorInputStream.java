package com.vimofthevine.underbudget.transactions.importer.parsers;

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;

import com.vimofthevine.underbudget.util.task.TaskProgress;

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
	    	progress.setMaximumValue(in.available());
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
			progress.add(read);
		}
	}

}
