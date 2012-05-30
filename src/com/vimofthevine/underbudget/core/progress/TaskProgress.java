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

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A <code>TaskProgress</code> object represents the percent-complete
 * of a long-running task (such as file access or analysis).
 * <p>
 * When the task progress is updated (i.e., via <code>reset()</code>,
 * <code>increasePercentCompleteBy()</code>, or <code>complete()</code>),
 * registered <code>TaskProgressListeners</code> are notified
 * of the progress update.
 * <p>
 * Task progress may be broken up into smaller sub-tasks, each with
 * their own progress. A task using a given <code>TaskProgress</code>
 * need only worry about his progress (0%-100%). The <code>TaskProgress</code>
 * representing a sub-task will update the parent <code>TaskProgress</code>
 * according to the fraction of the parent task's progress represented by
 * the sub-task (e.g., a sub-task representing 25% of a task will translate
 * 50% of the sub-task to 12.5% of the parent task)
 * <p>
 * The following demonstrates the usage of a <code>TaskProgress</code>
 * object to provide progress updates while performing a task:
 * <p>
 * <pre>
 * TaskProgress progress = new TaskProgress("Read file");
 * progress.reset();
 * 
 * for (int i=0; i<10; i++) {
 *     if (complexSubTask) {
 *         doSubTask(progress.createSubTask("Parse section", 10.0); // 10% of overall task
 *     }
 *     else {
 *         doStuff();
 *         progress.increasePercentCompleteBy(10.0); // 10% each time
 *     }
 * }
 * 
 * progress.complete();
 * </pre>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TaskProgress {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TaskProgress.class.getName());
	
	/**
	 * Throttling rate
	 */
	private static final int THROTTLING_RATE = 5;
	
	/**
	 * Name of the task whose progress is represented
	 */
	private String name;
	
	/**
	 * Parent task for this sub-task
	 */
	private TaskProgress parent;
	
	/**
	 * Portion of the parent task's progress
	 * represented by this sub-task
	 */
	private float fraction;
	
	/**
	 * List of registered listeners
	 */
	private List<TaskProgressListener> listeners;
	
	/**
	 * Current percent complete
	 */
	private Float percentComplete = 0f;
	
	/**
	 * Last reported percent complete
	 */
	private Float lastPercentage = 0f;
	
	/**
	 * Constructs a task progress object with the given
	 * task name.
	 * 
	 * @param name the name of the task whose progress
	 *         is represented
	 */
	public TaskProgress(String name)
	{
		this(name, null, 1.0f);
	}
	
	/**
	 * Constructs a sub-task progress object.
	 * <p>
	 * All progress updates on a sub-task are applied as
	 * a fractional amount to the parent task.
	 * 
	 * @param name     the name of the task whose progress
	 *                  is represented
	 * @param parent   parent task to which this sub-task belongs
	 * @param fraction fraction of the parent's progress
	 *                  represented by this sub-task
	 */
	private TaskProgress(String name, TaskProgress parent, float fraction)
	{
		logger.log(Level.FINER, "Initializing task progress updater for task " + name);
		
		this.name = name;
		this.parent = parent;
		this.fraction = fraction;
		
		percentComplete = 0f;
		lastPercentage = 0f;
		listeners = new ArrayList<TaskProgressListener>();
	}

	/**
	 * Registers a listener to receive task progress updates
	 * 
	 * @param listener task progress listener to register for updates
	 */
	final public void addTaskProgressListener(TaskProgressListener listener)
	{
		logger.log(Level.FINEST, "Registering " + listener + " for progress updates");
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from receiving task progress updates
	 * 
	 * @param listener task progress listener to unregister for updates
	 */
	final public void removeTaskProgressListener(TaskProgressListener listener)
	{
		logger.log(Level.FINEST, "Unregistering " + listener + " from receiving progress updates");
		listeners.remove(listener);
	}
	
	/**
	 * Notifies all registered listeners of a task progress update
	 * 
	 * @param value updated task progress value
	 */
	private void fireProgressUpdate(int value)
	{
		logger.log(Level.FINEST, "Firing progress update for " + value);
		
		for (TaskProgressListener listener : listeners)
		{
			listener.progressUpdate(name, value);
		}
	}

	/**
	 * Declares this task as not-yet-started, reseting the percent-complete to 0%.
	 * <p>
	 * This method is intended to be used only with top-level <code>TaskProgress</code>
	 * objects, allowing reuse between multiple executions of a particular task.
	 * Using <code>reset()</code> with sub-tasks could lead to the parent task's
	 * percent-complete exceeding 100%. This is because <code>reset()</code> does
	 * not propagate up to the parent task.
	 */
	public void reset()
	{
		logger.log(Level.FINER, "Resetting " + name + " task progress");
		
		percentComplete = 0f;
		lastPercentage = 0f;
		
		fireProgressUpdate(0);
	}

	/**
	 * Increases the percent-complete of this task
	 * by the specified percentage value.
	 * <p>
	 * If this task is a sub-task, the percent-complete of
	 * this task is applied to the parent task's percent-complete
	 * as a fraction of the parent task's progress.
	 * 
	 * @param percentage amount by which to increase this
	 *                    task's percent-complete
	 */
	public void increasePercentCompleteBy(float percentage)
	{
		if (parent == null)
		{
			logger.log(Level.FINEST, "Increasing " + name + " task percent-complete by " + percentage);
    		percentComplete += percentage;
    		
    		if (Math.abs(percentComplete - lastPercentage) >= THROTTLING_RATE)
    		{
    			lastPercentage = percentComplete;
    			fireProgressUpdate(percentComplete.intValue());
    		}
		}
		else
		{
			logger.log(Level.FINEST, "Increasing " + name + " sub-task percent-complete by " + percentage);
			percentComplete += percentage;
			
			float parentPercentage = percentage * fraction;
			logger.log(Level.FINEST, "Increasing parent task percent-complete by " + parentPercentage);
			parent.increasePercentCompleteBy(parentPercentage);
		}
	}
	
	/**
	 * Declares this task to be complete,
	 * updating the percent-complete to 100%.
	 * <p>
	 * If this task is a sub-task, the entire fraction of the
	 * parent task's progress represented by this sub-task is
	 * considered complete.
	 */
	public void complete()
	{
		if (parent == null)
		{
			logger.log(Level.FINER, name + " task is complete");
		
    		percentComplete = 100f;
    		lastPercentage = 100f;
		
    		fireProgressUpdate(100);
		}
		else
		{
			logger.log(Level.FINER, name + " sub-task is complete");
			
			float remaining = 100f - percentComplete;
			percentComplete = 100f;
			
			float parentPercentage = remaining * fraction;
			logger.log(Level.FINEST, "Increasing parent task by " + parentPercentage);
			parent.increasePercentCompleteBy(parentPercentage);
		}
	}
	
	/**
	 * Creates a sub-task to represent the specified fraction
	 * of this task's progress.
	 * <p>
	 * For example, to represent a task as three sub-tasks:
	 * <pre>
	 * TaskProgress progress = new TaskProgress("Overall task");
	 * TaskProgress subtask1 = progress.createSubTask("Setup", 10); // 10%
	 * TaskProgress subtask2 = progress.createSubTask("Read", 45); // 45%
	 * TaskProgress subtask3 = progress.createSubTask("Analyze", 45); // 45%
	 * </pre>
	 * 
	 * @param name     name of the sub-task
	 * @param percent  portion of this task's progress to be
	 *                  completed by the sub-task, as a percentage
	 *                  (e.g., 50%, 33%, 25%)
	 * @return new <code>TaskProgress</code> to represent the sub-task
	 */
	public TaskProgress createSubTask(String name, float percent)
	{
		return new TaskProgress(name, this, percent / 100);
	}
	
}
