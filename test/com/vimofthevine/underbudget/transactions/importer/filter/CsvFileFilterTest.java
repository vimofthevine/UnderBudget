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

package com.vimofthevine.underbudget.transactions.importer.filter;

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * Unit test case for the CsvFileFilter class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileFilterTest {

	/**
	 * Verifies that CSV files are correctly filtered
	 */
	@Test
	public final void testCheckString()
	{
		assertTrue(CsvFileFilter.check("field1,field2,field3", "value1,value2,value3"));
		assertTrue(CsvFileFilter.check("field1, field2, field3", "value1, value2, value3"));
		assertFalse(CsvFileFilter.check("field1,field2,field3", "value1,value2")); // Invalid line 2
		assertFalse(CsvFileFilter.check("field1,field2", "value1,value2")); // Not enough fields
		assertFalse(CsvFileFilter.check("<?xml?>", ""));
	}

}
