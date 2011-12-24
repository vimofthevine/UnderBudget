package com.vimofthevine.underbudget.stubs;

import com.vimofthevine.underbudget.util.task.TaskProgressListener;

/**
 * Stub task progress listener for unit testing
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubTaskProgressListener implements TaskProgressListener {

	/**
	 * The last value given as a progress update
	 */
	public int lastValue = 0;
	
	/**
	 * The nubmer of progress updates received
	 */
	public int numberOfUpdates = 0;
	
	@Override
    public void progressUpdate(int value)
    {
		lastValue = value;
		numberOfUpdates++;
    }
	
}
