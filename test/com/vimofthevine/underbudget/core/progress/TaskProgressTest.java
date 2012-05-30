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

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test case for the TaskProgress class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TaskProgressTest {
	
	/**
	 * The class under test
	 */
	TaskProgress task;
	
	/**
	 * Stub task progress listener
	 */
	StubTaskProgressListener listener;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		task = new TaskProgress("JUnit");
		listener = new StubTaskProgressListener();
		
		task.addTaskProgressListener(listener);
	}

	/**
	 * Verifies that progress updates are not sent
	 * out if the throttling threshold has not been met
	 */
	@Test
	public void testThrottlingRate()
	{
		task.increasePercentCompleteBy(4f);
		
		assertEquals(0, listener.lastValue);
		
		task.increasePercentCompleteBy(5f);
		
		assertEquals(9, listener.lastValue);
	}

	/**
	 * Verifies that the task progress is reset to 0
	 */
	@Test
	public void testReset()
	{
		task.increasePercentCompleteBy(15f);
		assertEquals(15, listener.lastValue);
		
		task.reset();
		assertEquals(0, listener.lastValue);
	}

	/**
	 * Verifies that the task progress is complete to 100
	 */
	@Test
	public void testComplete()
	{
		task.increasePercentCompleteBy(15f);
		assertEquals(15, listener.lastValue);
		
		task.complete();
		assertEquals(100, listener.lastValue);
	}
	
	/**
	 * Verifies that operations on sub-tasks propagate
	 * to the parent task as a fraction
	 */
	@Test
	public void testSubTask()
	{
		TaskProgress subTask = task.createSubTask("Test", 25f);
		
		subTask.increasePercentCompleteBy(50f);
		assertEquals(12, listener.lastValue); // 0.25 of 50% is 12.5%
		
		subTask.increasePercentCompleteBy(30f);
		assertEquals(20, listener.lastValue); // 0.25 of 80% is 20%
		
		subTask.complete();
		assertEquals(25, listener.lastValue);
		
		subTask.reset();
		assertEquals(25, listener.lastValue); // No change
	}
	
	/**
	 * Stub task progress listener
	 */
	class StubTaskProgressListener implements TaskProgressListener {

		public int lastValue = 0;
		
		@Override
        public void progressUpdate(String name, int value)
        {
			lastValue = value;
        }
		
	}

}
