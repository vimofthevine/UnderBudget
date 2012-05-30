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

package com.vimofthevine.underbudget.core.budget.assignment;

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * Unit test case for the ComparisonOperator class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ComparisonOperatorTest {

	/**
	 * Verifies that the getValueOf method
	 * is able to correctly create the operator
	 * enumeration based on different variants
	 * of valid inputs
	 */
	@Test
	public final void testGetValueOf()
	{
		assertEquals(ComparisonOperator.BEGINS_WITH, ComparisonOperator.getValueOf("begins_with"));
		assertEquals(ComparisonOperator.BEGINS_WITH, ComparisonOperator.getValueOf("beginswith"));
		assertEquals(ComparisonOperator.BEGINS_WITH, ComparisonOperator.getValueOf("BEGINS_WITH"));
		assertEquals(ComparisonOperator.BEGINS_WITH, ComparisonOperator.getValueOf("BEGINSWITH"));
		
		assertEquals(ComparisonOperator.EQUALS, ComparisonOperator.getValueOf("equals"));
		assertEquals(ComparisonOperator.EQUALS, ComparisonOperator.getValueOf("EQUALS"));
		
		assertEquals(ComparisonOperator.EQUALS_CASE, ComparisonOperator.getValueOf("equals_case"));
		assertEquals(ComparisonOperator.EQUALS_CASE, ComparisonOperator.getValueOf("equalscase"));
		assertEquals(ComparisonOperator.EQUALS_CASE, ComparisonOperator.getValueOf("EQUALS_CASE"));
		assertEquals(ComparisonOperator.EQUALS_CASE, ComparisonOperator.getValueOf("EQUALSCASE"));
		
		assertEquals(ComparisonOperator.CONTAINS, ComparisonOperator.getValueOf("contains"));
		assertEquals(ComparisonOperator.CONTAINS, ComparisonOperator.getValueOf("CONTAINS"));
		
		assertEquals(ComparisonOperator.ENDS_WITH, ComparisonOperator.getValueOf("ends_with"));
		assertEquals(ComparisonOperator.ENDS_WITH, ComparisonOperator.getValueOf("endswith"));
		assertEquals(ComparisonOperator.ENDS_WITH, ComparisonOperator.getValueOf("ENDS_WITH"));
		assertEquals(ComparisonOperator.ENDS_WITH, ComparisonOperator.getValueOf("ENDSWITH"));
		
		assertEquals(ComparisonOperator.NONE, ComparisonOperator.getValueOf("nomatch"));
	}

}
