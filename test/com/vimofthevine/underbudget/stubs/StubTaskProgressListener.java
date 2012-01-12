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
