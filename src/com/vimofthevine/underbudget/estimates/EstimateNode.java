package com.vimofthevine.underbudget.estimates;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeNode;

/**
 * Support for maintaining the hierarchical (parent
 * and children) relationship between estimates
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateNode {

	/**
	 * Tree node used as the basis for the structure
	 */
	private DefaultMutableTreeNode node;
	
	/**
	 * Constructor, given the associated estimate
	 * 
	 * @param estimate the estimate associated with this node
	 */
	public EstimateNode(Estimate estimate)
	{
		node = new DefaultMutableTreeNode();
		node.setUserObject(estimate);
	}
	
	/**
	 * Protected constructor, given the tree node
	 * 
	 * @param node tree node representing this
	 *             estimate node structure
	 */
	protected EstimateNode(TreeNode node)
	{
		this.node = (DefaultMutableTreeNode) node;
	}
	
	/**
	 * Returns the estimate associated with this node
	 * 
	 * @return associated estimate
	 */
	public Estimate getEstimate()
	{
		return (Estimate) node.getUserObject();
	}
	
	/**
	 * Adds an estimate node as a child node
	 * 
	 * @param child estimate to add as a child
	 */
	public void add(EstimateNode child)
	{
		// Add estimate as child node
		node.add(child.node);
	}
	
	/**
	 * Removes a child estimate
	 * 
	 * @param child child estimate to remove
	 */
	public void remove(EstimateNode child)
	{
		// Remove estimate as child node
		node.remove(child.node);
	}
	
	/**
	 * Returns the root estimate of this estimate's tree
	 * 
	 * @return root estimate to this estimate's tree
	 */
	public Estimate getRoot()
	{
		return (new EstimateNode(node.getRoot())).getEstimate();
	}
	
	/**
	 * Returns the parent estimate to this estimate
	 * 
	 * @return parent estimate to this estimate
	 */
	public Estimate getParent()
	{
		return (new EstimateNode(node.getParent())).getEstimate();
	}
	
	/**
	 * Returns the child estimate located at the given index
	 * 
	 * @param index the location of the child estimate
	 * @return child estimate located at the given index
	 */
	public Estimate getChildAt(int index)
	{
		return (new EstimateNode(node.getChildAt(index))).getEstimate();
	}
	
	/**
	 * Returns the number of children estimates
	 * under this estimate
	 * 
	 * @return number of child estimates
	 */
	public int getChildCount()
	{
		return node.getChildCount();
	}
	
	/**
	 * Checks if this estimate has any children estimates
	 * 
	 * @return true if this estimate has any child estimates
	 */
	public boolean hasChildren()
	{
		return ! node.isLeaf();
	}
	
}
