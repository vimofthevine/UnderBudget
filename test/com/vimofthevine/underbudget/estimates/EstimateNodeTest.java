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

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test case for the E class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateNodeTest {
	
	/**
	 * The class under test
	 */
	EstimateNode node;
	
	/**
	 * Child object
	 */
	EstimateNode child;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		node = new EstimateNode(new Estimate());
		child = new EstimateNode(new Estimate());
	}

	/**
	 * Verifies the adding of child estimates
	 */
	@Test
	public void testAdd()
	{
		assertFalse(node.hasChildren());
		assertEquals(0, node.getChildCount());
		
		node.add(child);
		
		assertTrue(node.hasChildren());
		assertEquals(1, node.getChildCount());
	}

	/**
	 * Verifies the removing of child estimates
	 */
	@Test
	public void testRemove()
	{
		node.add(child);
		
		assertTrue(node.hasChildren());
		assertEquals(1, node.getChildCount());
		
		node.remove(child);
		
		assertFalse(node.hasChildren());
		assertEquals(0, node.getChildCount());
	}

	/**
	 * Verifies the ability to retrieve the root
	 */
	@Test
	public void testGetRoot()
	{
		EstimateNode grandchild = new EstimateNode(new Estimate());
		
		node.add(child);
		child.add(grandchild);
		
		assertSame(node.getEstimate(), grandchild.getRoot());
	}

	/**
	 * Verifies the parent-child relationship
	 */
	@Test
	public void testGetParent()
	{
		node.add(child);
		
		assertSame(node.getEstimate(), child.getParent());
	}

	/**
	 * Verifies the ability to retrieve specific children
	 */
	@Test
	public void testGetChildAt()
	{
		EstimateNode sibling = new EstimateNode(new Estimate());
		
		node.add(child);
		node.add(sibling);
		
		assertSame(child.getEstimate(), node.getChildAt(0));
		assertSame(sibling.getEstimate(), node.getChildAt(1));
	}

}
