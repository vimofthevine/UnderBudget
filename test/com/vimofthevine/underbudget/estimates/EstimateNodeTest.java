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
