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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;

import com.vimofthevine.underbudget.core.budget.estimate.impact.BalanceImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.NoImpact;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.CategoryEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.EstimateBehavior;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;
import com.vimofthevine.underbudget.core.report.BalanceCalculationReport;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;
import com.vimofthevine.underbudget.core.report.ExpectedAmountRationale;

/**
 * The default estimate implementation.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="estimate")
public class DefaultEstimate implements Estimate {
	
	/**
	 * Name of this estimate
	 */
	@Element
	private String name;
	
	/**
	 * Impact to ending balances
	 */
	@Element(required=false)
	private BalanceImpact impact;
	
	/**
	 * Description of this estimate
	 */
	@Element(required=false)
	private String description;
	
	/**
	 * Estimated amount
	 */
	@Element(required=false)
	private Currency amount;
	
	/**
	 * Whether any more transactions are expected
	 */
	@Element(name="final",required=false)
	private Boolean noMoreTransactionsExpected;
	
	/**
	 * Behavior of this estimate
	 */
	@Element(required=false)
	private EstimateBehavior behavior;
	
	/**
	 * Child estimates
	 */
	@ElementList(name="estimates",required=false)
	private ArrayList<DefaultEstimate> children;
	
	/**
	 * Transactions that have been assigned to
	 * this estimate
	 */
	private final List<AnalyzableTransaction> transactions;
	
	/**
	 * Constructs a new estimate with no balance
	 * impact, definition, or behavior. This is only
	 * useful for creating root estimates.
	 */
	public DefaultEstimate()
	{
		this.name = "Root";
		this.description = null;
		this.amount = null;
		this.noMoreTransactionsExpected = null;
		this.behavior = null;
		this.impact = null;
		
		transactions = new ArrayList<AnalyzableTransaction>();
	}
	
	/**
	 * Constructs a new estimate with the given
	 * impact, definition, and behavior.
	 * 
	 * @param behavior estimate behavior
	 * @param impact   balance impact
	 */
	public DefaultEstimate(String name, String description, Currency amount,
		boolean isFinal, EstimateBehavior behavior, BalanceImpact impact)
	{
		this.name = name;
		this.description = "".equals(description) ? null : description;
		this.amount = amount;
		this.noMoreTransactionsExpected = isFinal ? true : null;
		this.behavior = (behavior instanceof CategoryEstimateBehavior) ? null : behavior;
		this.impact = (impact instanceof NoImpact) ? null : impact;
		
		transactions = new ArrayList<AnalyzableTransaction>();
	}
	
	@Override
    public Iterator<Estimate> iterator()
    {
		return new DefaultEstimateIterator(this);
    }
	
	List<DefaultEstimate> getChildren()
	{
		return children;
	}
	
	// -- Methods from the MutableEstimate interface
	
	@Override
	public void addChild(MutableEstimate estimate)
	{
		if ( ! (estimate instanceof DefaultEstimate))
			throw new IllegalArgumentException("Cannot add child estimate of type " + estimate.getClass().getName());
		
		if (children == null)
		{
			children = new ArrayList<DefaultEstimate>();
		}
		
		children.add((DefaultEstimate) estimate);
	}
	
	@Override
	public void removeChild(MutableEstimate estimate)
	{
		if ( ! (estimate instanceof DefaultEstimate))
			throw new IllegalArgumentException("Cannot remove child estimate of type " + estimate.getClass().getName());
		
		if (children != null)
		{
			children.remove((DefaultEstimate) estimate);
		}
	}
	
	@Override
	public void rename(String name)
	{
		this.name = name;
	}
	
	@Override
	public void describe(String description)
	{
		this.description = "".equals(description) ? null : description;
	}
	
	@Override
	public void setEstimatedAmount(Currency amount)
	{
		this.amount = amount;
	}
	
	@Override
	public void hasMoreTransactionsExpected(boolean more)
	{
		this.noMoreTransactionsExpected = (! more) ? true : null;
	}
	
	@Override
	public void specifyImpact(BalanceImpact impact)
	{
		this.impact = (impact instanceof NoImpact) ? null : impact;
	}
	
	@Override
	public void specifyBehavior(EstimateBehavior behavior)
	{
		if (children != null && children.size() > 0)
		{
			this.behavior = null;
		}
		else
		{
			this.behavior = (behavior instanceof CategoryEstimateBehavior)
				? null : behavior;
		}
	}

	// -- Methods from the DisplayableEstimate interface
	
	@Override
	public String getName()
	{
		return name;
	}
	
	@Override
	public String getDescription()
	{
		return (description == null) ? "" : description;
	}
	
	@Override
	public Currency getEstimatedAmount()
	{
		return amount.clone();
	}
	
	@Override
	public boolean moreTransactionsAreExpected()
	{
		return (noMoreTransactionsExpected == null)
			? true : ! noMoreTransactionsExpected;
	}
	
	/*
	@Override
	public BalanceImpact getImpact()
	{
		return (impact == null) ? new NoImpact() : impact;
	}
	
	@Override
	public EstimateBehavior getBehavior()
	{
		return (behavior == null)
			? new CategoryEstimateBehavior() : behavior;
	}
	*/
	
	// -- Methods from the AnalyzableEstimate interface
	
	@Override
	public void clearAssignedTransactions()
	{
		synchronized(transactions)
		{
			transactions.clear();
		}
	}

	@Override
    public void assignTransaction(AnalyzableTransaction transaction)
    {
		synchronized(transactions)
		{
			transactions.add(transaction);
		}
    }
	
	/**
	 * Recursively calculates the estimated amount as the sum
	 * of its child estimate's estimated amounts, or the
	 * user-defined estimated amount if it has no children.
	 * <p>
	 * If performance becomes a concern, this could be
	 * calculated once and cached, rather than re-calculating
	 * each time it is needed. The cache would be cleared along
	 * with the assigned transactions.
	 * 
	 * @return estimated amount of this estimate
	 */
	private Currency calculateEstimatedAmount()
	{
		// Create a zero amount
		Currency estimated = amount.clone();
		estimated.decreaseBy(amount);
		
		// If a category of estimates
		if (children.size() > 0)
		{
			for (DefaultEstimate estimate : children)
			{
				estimated.increaseBy(estimate.calculateEstimatedAmount());
			}
		}
		else
		{
			estimated.increaseBy(amount);
		}
		
		return estimated;
	}
	
	/**
	 * Recursively calculates the actual amount as the sum
	 * of its child estimate's actual amounts, or the sum
	 * of its assigned transaction amounts if it has no
	 * children.
	 * <p>
	 * If performance becomes a concern, this could be
	 * calculated once and cached, rather than re-calculating
	 * each time it is needed. The cache would be cleared along
	 * with the assigned transactions.
	 * 
	 * @return actual amount of this estimate
	 */
	private Currency calculateActualAmount()
	{
		// Create a zero amount
		Currency actual = amount.clone();
		actual.decreaseBy(amount);
		
		// If a category of estimates
		if (children.size() > 0)
		{
			for (DefaultEstimate estimate : children)
			{
				actual.increaseBy(estimate.calculateActualAmount());
			}
		}
		else
		{
			synchronized(transactions)
			{
				for (AnalyzableTransaction transaction : transactions)
				{
					transaction.addTransferAmountTo(actual);
				}
			}
		}
		
		return actual;
	}

	@Override
    public void reportProgress(EstimateProgressReport report)
    {
		// Only applies to leaf nodes, not categories
		if (children.size() == 0)
		{
    		Currency actual = calculateActualAmount();
    		
    		behavior.report(this, amount, actual, report);
    		report.recordProgress(this, amount, actual);
		}
    }

	@Override
    public void addValuesTo(Currency estimated, Currency actual,
            Currency expected, BalanceCalculationReport report)
    {
		// Only applies to leaf nodes, not categories
		if (children.size() == 0)
		{
    		Currency estimatedAmount = calculateEstimatedAmount();
    		Currency actualAmount = calculateActualAmount();
    		
    		impact.apply(estimatedAmount, estimated);
    		impact.apply(actualAmount, actual);
    		
    		// If finished, use actual amount
    		if (noMoreTransactionsExpected)
    		{
    			impact.apply(actualAmount, expected);
    			report.record(this, actualAmount, ExpectedAmountRationale.NO_MORE_TRANSACTIONS);
    		}
    		// If over-budget, use actual amount
    		else if (actualAmount.compareTo(estimatedAmount) > 0)
    		{
    			impact.apply(actualAmount, expected);
    			report.record(this, actualAmount, ExpectedAmountRationale.ACTUAL_EXCEEDS_ESTIMATED);
    		}
    		// Else under-budget, use estimated amount (extrapolate)
    		else
    		{
    			impact.apply(estimatedAmount, expected);
    			report.record(this, estimatedAmount, ExpectedAmountRationale.ESTIMATED_EXCEEDS_ACTUAL);
    		}
		}
    }
	
	@Override
	public int hashCode()
	{
		int result = 12;
		result = result * 31 + name.hashCode();
		result = result * 31 + impact.hashCode();
		result = result * 31 + behavior.hashCode();
		result = result * 31 + (description == null ? 0 : description.hashCode());
		result = result * 31 + (amount == null ? 0 : amount.hashCode());
		result = result * 31 + (noMoreTransactionsExpected == null ? 0 : noMoreTransactionsExpected.hashCode());
		result = result * 31 + (children == null ? 0 : children.hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		
		if ( ! (obj instanceof DefaultEstimate))
			return false;
		
		DefaultEstimate that = (DefaultEstimate) obj;
		
		return this.name.equals(that.name)
			&& (this.impact == null ? that.impact == null : this.impact.equals(that.impact))
			&& (this.description == null ? that.description == null : this.description.equals(that.description))
			&& (this.amount == null ? that.amount == null : this.amount.equals(that.amount))
			&& (this.noMoreTransactionsExpected == null ? that.noMoreTransactionsExpected == null : this.noMoreTransactionsExpected.equals(that.noMoreTransactionsExpected))
			&& (this.children == null ? that.children == null : this.children.equals(that.children))
		;
	}
	
	@Override
	public String toString()
	{
		return "[" + name + ": impact=" + impact + ", amount=" + amount
			+ ", description=" + description + ", final=" + noMoreTransactionsExpected
			+ ", behavior=" + behavior
			+ ", children=" + (children == null ? 0 : children.size()) + "]";
	}

}
