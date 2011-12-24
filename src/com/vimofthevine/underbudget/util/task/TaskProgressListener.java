package com.vimofthevine.underbudget.util.task;

/**
 * Interface for task progress listeners
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TaskProgressListener {
	
	/**
	 * Receives and responds to an updated task progress value
	 * 
	 * @param value updated task progress value
	 */
	public void progressUpdate(int value);

}
