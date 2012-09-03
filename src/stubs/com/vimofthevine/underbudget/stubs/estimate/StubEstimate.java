package com.vimofthevine.underbudget.stubs.estimate;

import java.util.ArrayList;
import java.util.List;

import com.vimofthevine.underbudget.core.assignment.ActualFigure;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.BalanceImpact;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;

public class StubEstimate implements Estimate,
MutableEstimate {

	/**
	 * Estimate name
	 */
	private String name;

	/**
	 * Estimated amount
	 */
	private CashCommodity amount;
	
	private String description;
	private SimpleDate dueDate;
	private boolean complete;
	private EstimateType type;

	/**
	 * Child estimates
	 */
	private final List<StubEstimate> children;
	
	/**
	 * Parent estimate
	 */
	private StubEstimate parent;

	/**
	 * Constructs a new estimate
	 * 
	 * @param name
	 *            estimate name
	 * @param amount
	 *            estimate amount
	 */
	public StubEstimate(String name, CashCommodity amount)
	{
		this.name = name;
		this.amount = amount;
		description = "";
		dueDate = null;
		complete = false;
		type = EstimateType.EXPENSE;
		
		children = new ArrayList<StubEstimate>();
	}

	public void add(StubEstimate child)
	{
		children.add(child);
		child.parent = this;
	}
	
	public void remove(StubEstimate child)
	{
		children.remove(child);
	}

	@Override
	public int getChildCount()
	{
		return children.size();
	}

	@Override
	public Estimate getChildAt(int index)
	{
		return children.get(index);
	}

	@Override
	public EstimateProgress getProgress(final ActualFigure actual, CurrencyCalculator calculator)
	{
		return new EstimateProgress() {

			@Override
			public CashCommodity getEstimatedAmount()
			{
				return amount;
			}

			@Override
			public CashCommodity getActualAmount()
			{
				return actual.getAmount();
			}

			@Override
			public String getNotice()
			{
				return "";
			}

			@Override
			public boolean isHealthy()
			{
				return actual.getAmount().getValue().compareTo(amount.getValue()) < 0;
			}

		};
	}

	@Override
	public Estimate createChild()
	{
		StubEstimate child = (children.size() == 0)
			? new StubEstimate(name, amount)
			: new StubEstimate("New estimate", Commodity.zero(amount.getCurrency()));
		add(child);
		return child;
	}

	@Override
	public void delete()
	{
		parent.remove(this);
	}
	
	@Override
	public boolean isRoot()
	{
		return (parent == null);
	}

	@Override
	public Estimate getParent()
	{
		return parent;
	}

	@Override
	public int indexOf(Estimate child)
	{
		return children.indexOf(child);
	}

	@Override
	public BalanceImpact getImpact(final ActualFigure actual, CurrencyCalculator calculator)
	{
		return new BalanceImpact() {

			@Override
            public CashCommodity getEstimatedImpact()
            {
				return amount;
            }

			@Override
            public CashCommodity getActualImpact()
            {
				return actual.getAmount();
            }

			@Override
            public CashCommodity getExpectedImpact()
            {
				return amount;
            }

			@Override
            public String getNotice()
            {
				return "";
            }
			
		};
	}
	
	@Override
	public int hashCode()
	{
		int result = 2342;
		result = result * 31 + name.hashCode();
		result = result * 31 + amount.hashCode();
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof StubEstimate))
			return false;
		
		StubEstimate that = (StubEstimate) obj;
		return this.name.equals(that.name)
			&& this.amount.equals(that.amount);
	}
	
	@Override
	public String toString()
	{
		return name;
	}

    @Override
    public void setDefinition(EstimateDefinition definition)
    {
    	name = definition.getName();
    	description = definition.getDescription();
    	amount = definition.getAmount();
    	type = definition.getType();
    	dueDate = definition.getDueDate();
    	complete = definition.isComplete();
    }

    @Override
    public EstimateDefinition getDefinition()
    {
    	return new EstimateDefinition() {

			@Override
            public String getName()
            {
				return name;
            }

			@Override
            public String getDescription()
            {
				return description;
            }

			@Override
            public CashCommodity getAmount()
            {
				return amount;
            }

			@Override
            public SimpleDate getDueDate()
            {
	            return dueDate;
            }

			@Override
            public EstimateType getType()
            {
				return type;
            }

			@Override
            public boolean isComplete()
            {
	            return complete;
            }
    		
    	};
    }

}
