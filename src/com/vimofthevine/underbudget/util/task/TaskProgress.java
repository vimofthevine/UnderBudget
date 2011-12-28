package com.vimofthevine.underbudget.util.task;

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 * Representation of a long-running task's progress
 * (i.e., percent-complete). Provides support for
 * notifying registered listeners of task progress updates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TaskProgress {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TaskProgress.class.getName());
	
	/**
	 * List of registered listeners
	 */
	List<TaskProgressListener> listeners;
	
	/**
	 * Update throttling rate
	 */
	private int throttlingRate = 0;
	
	/**
	 * Current percent complete
	 */
	private Float currentPercentage = 0f;
	
	/**
	 * Last reported percent complete
	 */
	private Float lastPercentage = 0f;
	
	/**
	 * Maximum value
	 */
	private int maxValue = 0;
	
	/**
	 * Current value
	 */
	private int currentValue = 0;
	
	/**
	 * Class constructor
	 */
	public TaskProgress()
	{
		logger.log(Level.FINER, "Initializing task progress updater");
		
		listeners = new ArrayList<TaskProgressListener>();
	}

	/**
	 * Registers a listener to receive task progress updates
	 * 
	 * @param listener task progress listener to register for updates
	 */
	public void addTaskProgressListener(TaskProgressListener listener)
	{
		logger.log(Level.FINEST, "Adding listener registration");
		
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from receiving task progress updates
	 * 
	 * @param listener task progress listener to unregister for updates
	 */
	public void removeTaskProgressListener(TaskProgressListener listener)
	{
		logger.log(Level.FINEST, "Removing listener registration");
		
		listeners.remove(listener);
	}
	
	/**
	 * Notifies all registered listeners of a task progress update
	 * 
	 * @param value updated task progress value
	 */
	public void fireProgressUpdate(int value)
	{
		logger.log(Level.FINEST, "Firing progress update for " + value);
		
		for (TaskProgressListener listener : listeners)
		{
			listener.progressUpdate(value);
		}
	}
	
	/**
	 * Defines a throttling rate at which progress updates will occur
	 *
	 * @param rate minimum progress percentage between consecutive progress
	 *             updates
	 */
	public void setThrottlingRate(int rate)
	{
		logger.log(Level.FINEST, "Setting throttling rate to " + rate);
		
		throttlingRate = rate;
	}
	
	/**
	 * Defines the maximum value that can be supplied. This
	 * allows use of the progress updater without calculating
	 * percentages.
	 * 
	 * @param max maximum value
	 */
	public void setMaximumValue(int max)
	{
		logger.log(Level.FINEST, "Setting maximum value to " + max);
		
		maxValue = max;
	}

	/**
	 * Resets the current task completion percentage, notifying all
	 * listeners
	 */
	public void reset()
	{
		logger.log(Level.FINEST, "Resetting task progress");
		
		currentPercentage = 0f;
		lastPercentage = 0f;
		currentValue = 0;
		
		fireProgressUpdate(0);
	}

	/**
	 * Adds the given percentage to the current task completion percentage,
	 * notifying all listeners if the throttling rate has been satisfied
	 *
	 * @param value progress percentage by which the current task completion
	 *              has increased
	 */
	public void add(float value)
	{
		logger.log(Level.FINEST, "Adding " + value + " to current task percentage");

		currentPercentage += value;
		
		if (Math.abs(currentPercentage - lastPercentage) > throttlingRate)
		{
			lastPercentage = currentPercentage;
			fireProgressUpdate(currentPercentage.intValue());
		}
	}
	
	/**
	 * Increments the current value used to calculate current
	 * percentage, notifying all listeners if the throttling rate
	 * has been satisfied
	 * 
	 * @param value progress increment by which the current task
	 *              completion has increased
	 */
	public void add(int value)
	{
		logger.log(Level.FINEST, "Adding " + value + " to current value");
		
		currentValue += value;
		
		if (currentValue > maxValue)
		{
			currentValue = maxValue;
		}
		
		currentPercentage = (float) currentValue * 100 / (float) maxValue;
		
		if (Math.abs(currentPercentage - lastPercentage) > throttlingRate)
		{
			lastPercentage = currentPercentage;
			fireProgressUpdate(currentPercentage.intValue());
		}
	}

	/**
	 * Marks the current task as completed, notifying all listeners
	 */
	public void complete()
	{
		logger.log(Level.FINEST, "Marking task as complete");
		
		currentPercentage = 100f;
		lastPercentage = 100f;
		currentValue = maxValue;
		
		fireProgressUpdate(100);
	}
	
}
