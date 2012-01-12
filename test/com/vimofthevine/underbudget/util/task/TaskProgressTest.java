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

package com.vimofthevine.underbudget.util.task;

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
	TaskProgress progress;
	
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
		progress = new TaskProgress();
		listener = new StubTaskProgressListener();
		
		progress.addTaskProgressListener(listener);
	}

	/**
	 * Verifies that progress updates are not sent
	 * out if the throttling threshold has not been met
	 */
	@Test
	public void testThrottlingRate()
	{
		progress.setThrottlingRate(10);
		progress.add(5f);
		
		assertEquals(0, listener.lastValue);
		
		progress.add(6f);
		
		assertEquals(11, listener.lastValue);
	}

	/**
	 * Verifies that the task progress is reset to 0
	 */
	@Test
	public void testReset()
	{
		progress.add(15f);
		assertEquals(15, listener.lastValue);
		
		progress.reset();
		assertEquals(0, listener.lastValue);
	}

	/**
	 * Verifies that the task progress is complete to 100
	 */
	@Test
	public void testComplete()
	{
		progress.add(15f);
		assertEquals(15, listener.lastValue);
		
		progress.complete();
		assertEquals(100, listener.lastValue);
	}
	
	/**
	 * Stub task progress listener
	 */
	class StubTaskProgressListener implements TaskProgressListener {

		public int lastValue = 0;
		
		@Override
        public void progressUpdate(int value)
        {
			lastValue = value;
        }
		
	}

}
