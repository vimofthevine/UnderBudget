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

package com.vimofthevine.underbudget.core.budget.estimate;

import static org.junit.Assert.*;

import java.util.Iterator;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test case for the DefaultEstimateIterator class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultEstimateIteratorTest {
	
	/**
	 * Root estimate
	 */
	private DefaultEstimate root;
	
	/**
	 * Sets up the test case objects
	 */
	@Before
	public final void setUp()
	{
		root = new DefaultEstimate();
		
		DefaultEstimate num1 = new DefaultEstimate("1", "", null, false, null, null);
		DefaultEstimate num2 = new DefaultEstimate("2", "", null, false, null, null);
		DefaultEstimate num3 = new DefaultEstimate("3", "", null, false, null, null);
		DefaultEstimate num4 = new DefaultEstimate("4", "", null, false, null, null);
		DefaultEstimate num5 = new DefaultEstimate("5", "", null, false, null, null);
		DefaultEstimate num6 = new DefaultEstimate("6", "", null, false, null, null);
		DefaultEstimate num7 = new DefaultEstimate("7", "", null, false, null, null);
		DefaultEstimate num8 = new DefaultEstimate("8", "", null, false, null, null);
		DefaultEstimate num9 = new DefaultEstimate("9", "", null, false, null, null);
		
		root.addChild(num1);
		num1.addChild(num2);
		num2.addChild(num3);
		num2.addChild(num4);
		num1.addChild(num5);
		root.addChild(num6);
	    num6.addChild(num7);
	    num6.addChild(num8);
	    num8.addChild(num9);
	}

	/**
	 * Verifies that the default estimate iterator
	 * iterates in a depth-first fashion.
	 */
	@Test
	public final void test()
	{
		Iterator<Estimate> iterator = root.iterator();
		int i = 0;
		
		while (iterator.hasNext())
		{
			String name = (i == 0) ? "Root" : String.valueOf(i);
			i++;
			assertEquals(name, iterator.next().getName());
		}
	}

}
