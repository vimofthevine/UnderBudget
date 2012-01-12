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

package com.vimofthevine.underbudget.estimates;

import static org.junit.Assert.*;

import java.math.BigDecimal;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.estimates.rules.Rule;

/**
 * Unit test case for the Estimate class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateTest {

	/**
	 * The class under test
	 */
	Estimate estimate;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		estimate = new Estimate();
	}
	
	/**
	 * Verifies that estimates with children
	 * are considered category estimates
	 */
	@Test
	public void testIsCategory()
	{
		assertFalse(estimate.isCategory());
		
		estimate.add(new Estimate());
		
		assertTrue(estimate.isCategory());
	}

	/**
	 * Verifies the adding and removing of rules
	 * to the estimate's rule list
	 */
	@Test
	public void testAddAndRemoveRule()
	{
		assertEquals(0, estimate.rules.size());
		
		Rule rule = new Rule();
		
		estimate.addRule(rule);
		assertEquals(1, estimate.rules.size());
		
		estimate.removeRule(rule);
		assertEquals(0, estimate.rules.size());
	}

	/**
	 * Verifies that category estimate amount
	 * is the sum of its child category's amounts
	 */
	@Test
	public void testGetAmount()
	{
		Estimate child = new Estimate();
		Estimate sibling = new Estimate();
		
		estimate.add(child);
		estimate.add(sibling);
		
		child.setAmount(new BigDecimal("12.34"));
		sibling.setAmount(new BigDecimal("20.00"));
		
		assertEquals(new BigDecimal("12.34"), child.getAmount());
		assertEquals(new BigDecimal("32.34"), estimate.getAmount());
	}

}
