package com.vimofthevine.underbudget.estimates;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;
import java.util.Observable;

import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.transactions.Transaction;

/**
 * Representation of a user-defined estimate, which
 * may be an income or an expense estimate
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Estimate extends Observable {
	
	/**
	 * Estimate hierarchy structure support
	 */
	protected EstimateNode node;
	
	/**
	 * The identifying name of this estimate
	 */
	protected String name = "";
	
	/**
	 * User-defined notes about this estimate
	 */
	protected String notes = "";
	
	/**
	 * Rules used to match transactions against this estimate
	 */
	protected List<Rule> rules;
	
	/**
	 * The expected amount for this estimate
	 */
	protected BigDecimal amount = new BigDecimal("0");
	
	/**
	 * Indication that all transactions against this estimate
	 * have occurred and that only actual data should be considered
	 */
	protected boolean isFinal = false;
	
	/**
	 * Transactions that have been matched against this estimate
	 */
	protected List<Transaction> transactions;
	
	/**
	 * Synchronization object
	 */
	private Object sync = new Object();
	
	/**
	 * Default constructor, initializing the tree
	 * node
	 */
	public Estimate()
	{
		node = new EstimateNode(this);
		rules = new ArrayList<Rule>();
		transactions = new ArrayList<Transaction>();
	}
	
	/**
	 * Adds an estimate as a child estimate
	 * 
	 * @param child estimate to add as a child
	 */
	public void add(Estimate child)
	{
		node.add(child.node);
		setChanged();
	}
	
	/**
	 * Removes a child estimate
	 * 
	 * @param child child estiamte to remove
	 */
	public void remove(Estimate child)
	{
		node.remove(child.node);
		setChanged();
	}
	
	/**
	 * Returns the number of child estimates
	 * 
	 * @return number of child estimates
	 */
	public int getChildCount()
	{
		return node.getChildCount();
	}
	
	/**
	 * Returns the child estimate located at the given index
	 * 
	 * @param index the location of the child estimate
	 * @return child estimate located at the given index
	 */
	public Estimate getChildAt(int index)
	{
		return node.getChildAt(index);
	}
	
	/**
	 * Checks if this estimate is a category of
	 * estimate (i.e., it has child estimates)
	 * 
	 * @return true if this estimate has children
	 */
	public boolean isCategory()
	{
		return node.hasChildren();
	}
	
	/**
	 * Returns the identifying name of this estimate
	 * 
	 * @return identification name of this estimate
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 * Sets the identifying name of this estimate.
	 * If the new name is different than the current
	 * name, the estimate is marked as changed.
	 * 
	 * @param name identification name of this estimate
	 */
	public void setName(String name)
	{
		if ( ! this.name.equals(name))
		{
			this.name = name;
			setChanged();
		}
	}
	
	/**
	 * Returns the user-notes for this estimate
	 * 
	 * @return user-notes for this estimate
	 */
	public String getNotes()
	{
		return notes;
	}
	
	/**
	 * Sets the user-notes for this estimate. If
	 * the new notes are different than the current
	 * user-notes, the estimate is marked as changed.
	 * 
	 * @param notes user-notes for this estimate
	 */
	public void setNotes(String notes)
	{
		if ( ! this.notes.equals(notes))
		{
			this.notes = notes;
			setChanged();
		}
	}
	
	/**
	 * Retrieves the rule list for matching transactions
	 * against this estimate
	 * 
	 * @return list of rules for matching transactions
	 */
	public List<Rule> getRules()
	{
		return rules;
	}
	
	/**
	 * Adds a new rule to the estimate's rule list
	 * 
	 * @param rule new rule for matching transactions
	 */
	public void addRule(Rule rule)
	{
		if ( ! rules.contains(rule))
		{
			rules.add(rule);
			setChanged();
		}
	}
	
	/**
	 * Removes a rule from the estimate's rule list
	 * 
	 * @param rule rule to remove from the rule list
	 */
	public void removeRule(Rule rule)
	{
		if (rules.contains(rule))
		{
			rules.remove(rule);
			setChanged();
		}
	}
	
	/**
	 * Returns the expected amount of this estimate,
	 * or the sum of all child estimate amounts if
	 * this estimate is a category of estimates
	 * 
	 * @return expected amount of this estimate, or
	 *         the sum of all child estimate amounts
	 */
	public BigDecimal getAmount()
	{
		if ( ! isCategory())
			return amount;
		else
		{
			BigDecimal amt = new BigDecimal("0");
			
			for (int i=0; i<node.getChildCount(); i++)
			{
				amt = amt.add(node.getChildAt(i).getAmount());
			}
			
			return amt;
		}
	}
	
	/**
	 * Sets the expected amount of this estimate. If
	 * the new amount is different than the current
	 * amount, the estimate is marked as changed and
	 * observers are notified that the amount has changed.
	 * 
	 * @param amount expected amount of this estimate
	 */
	public void setAmount(BigDecimal amount)
	{
		// Don't do anything if this estimate is a category
		if (isCategory())
			return;
		
		if ( ! this.amount.equals(amount))
		{
			this.amount = amount;
			setChanged();
		}
	}
	
	/**
	 * Checks if this estimate has been designated as final
	 * 
	 * @return true if this estimate has been marked as final
	 */
	public boolean isFinal()
	{
		return isFinal;
	}
	
	/**
	 * Designate this estimate as being final. A final
	 * estimate indicates that all transactions against the
	 * estimate have occurred and that only actual data is
	 * considered. If the new final designation is different
	 * than the current final designation, the estimate is
	 * marked as changed.
	 * 
	 * @param isFinal whether the estimate is final
	 */
	public void setFinal(boolean isFinal)
	{
		if (this.isFinal != isFinal)
		{
			this.isFinal = isFinal;
			setChanged();
		}
	}
	
	/**
	 * Clears the list of matched transactions
	 */
	public void clear()
	{
		synchronized(sync)
		{
			transactions = new ArrayList<Transaction>();
			setChanged();
		}
	}
	
	/**
	 * Adds a transaction to the matched transactions list
	 * 
	 * @param transaction transaction that has been matched
	 *                    against this estimate
	 */
	public void addTransaction(Transaction transaction)
	{
		synchronized(sync)
		{
			transactions.add(transaction);
			setChanged();
		}
	}
	
	/**
	 * Returns the list of transactions that have been
	 * matched against this estimate
	 * 
	 * @return list of matched transactions
	 */
	public List<Transaction> getTransactions()
	{
		return transactions;
	}
	
	/**
	 * Calculates the actual credits/debits against
	 * this estimate, based on transactions that have
	 * been matched against this estimate
	 * 
	 * @return actual net amount
	 */
	public BigDecimal getActualAmount()
	{
		BigDecimal amount = new BigDecimal("0");
		
		// Get sum of all transaction amounts
		if ( ! isCategory())
		{
			synchronized(sync)
			{
				for (Transaction transaction : transactions)
				{
					amount = amount.add(transaction.value);
				}
			}
		}
		// Else get the sum of all child estimate actuals
		else
		{
			for (int i=0; i<getChildCount(); i++)
			{
				amount = amount.add(getChildAt(i).getActualAmount());
			}
		}
		
		return amount;
	}
	
	/**
	 * Returns the estimate name as the string
	 * representation of this estimate
	 * 
	 * @return string representation of this estimate
	 */
	@Override
	public String toString()
	{
		return name;
	}
	
}
