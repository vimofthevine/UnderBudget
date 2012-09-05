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

package com.vimofthevine.underbudget.xml.estimate;

import java.util.ArrayList;
import java.util.Random;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Version;
import org.simpleframework.xml.convert.Convert;
import org.simpleframework.xml.core.Commit;

import com.vimofthevine.underbudget.core.assignment.ActualFigure;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.BalanceImpact;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.core.estimate.impact.CategoryImpact;
import com.vimofthevine.underbudget.core.estimate.impact.ExpenseImpact;
import com.vimofthevine.underbudget.core.estimate.impact.IncomeImpact;
import com.vimofthevine.underbudget.core.estimate.impact.TransferImpact;
import com.vimofthevine.underbudget.core.estimate.progress.CategoryProgress;
import com.vimofthevine.underbudget.core.estimate.progress.ExpenseProgress;
import com.vimofthevine.underbudget.core.estimate.progress.IncomeProgress;
import com.vimofthevine.underbudget.core.estimate.progress.TransferProgress;
import com.vimofthevine.underbudget.xml.currency.CommodityConverter;
import com.vimofthevine.underbudget.xml.date.XmlDate;

/**
 * A <code>MutableEstimate</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlEstimate implements MutableEstimate {
	
	/**
	 * Estimate definition version
	 */
	@Version(revision=1.1)
	private double version;
	
	/**
	 * Estimate ID
	 */
	@Attribute
	private long uid;
	
	/**
	 * Estimate name
	 */
	@Element
	private String name;
	
	/**
	 * Estimate description
	 */
	@Element(required=false)
	private String description;
	
	/**
	 * Estimated amount
	 */
	@Element(required=false)
	@Convert(CommodityConverter.class)
	private CashCommodity amount;
	
	/**
	 * Estimate type
	 */
	@Element(required=false)
	private EstimateType type;
	
	/**
	 * Due date
	 */
	@Element(name="due-date", required=false)
	private XmlDate dueDate;
	
	/**
	 * Complete
	 */
	@Element(required=false)
	private boolean complete;
	
	/**
	 * Child estimates
	 */
	@ElementList(name="estimates",entry="estimate",required=false)
	private ArrayList<XmlEstimate> children;
	
	/**
	 * Parent estimate
	 */
	private XmlEstimate parent;
	
	XmlEstimate()
	{
		System.out.println("constructing estimate with no parameters");
		uid = new Random().nextLong();
		type = EstimateType.CATEGORY; // Default to category
		children = new ArrayList<XmlEstimate>();
	}
	
	/**
	 * Constructs a new XML serializable estimate
	 * wrapper around the given estimate.
	 * 
	 * @param original original estimate
	 */
	public XmlEstimate(Estimate original)
	{
		EstimateDefinition definition = original.getDefinition();
		
		uid = original.getId();
		name = definition.getName();
		description = definition.getDescription();
		amount = definition.getAmount();
		type = definition.getType();
		dueDate = (definition.getDueDate() == null) ? null
			: new XmlDate(definition.getDueDate());
		complete = definition.isComplete();
		
		children = new ArrayList<XmlEstimate>();
		
		for (int i=0; i<original.getChildCount(); i++)
		{
			Estimate estimate = original.getChildAt(i);
			children.add(i, new XmlEstimate(estimate));
		}
	}
	
	XmlEstimate(
		@Attribute(name="uid") long uid,
		@Element(name="name") String name,
		@Element(name="description") String description,
		@Element(name="amount") CashCommodity amount,
		@Element(name="type") EstimateType type,
		@Element(name="due-date") XmlDate dueDate,
		@Element(name="complete") boolean complete,
		@ElementList(name="estimates") ArrayList<XmlEstimate> children
	)
	{
		this.uid = uid;
		this.name = name;
		this.description = description;
		this.amount = amount;
		this.type = (type == null) ? EstimateType.CATEGORY : type;
		this.dueDate = dueDate;
		this.complete = complete;
		this.children = children;
	}
	
	@Commit
	private void setParentReferences()
	{
		for (XmlEstimate child : children)
		{
			child.parent = this;
		}
	}
	
	void add(XmlEstimate child)
	{
		children.add(child);
		child.parent = this;
	}
	
	void remove(XmlEstimate child)
	{
		children.remove(child);
	}
	
	@Override
	public long getId()
	{
		return uid;
	}

	@Override
    public EstimateDefinition getDefinition()
    {
		return new EstimateDefinition() {
			@Override
            public String getName() { return name; }

			@Override
            public String getDescription() { return description; }

			@Override
            public CashCommodity getAmount() { return amount; }

			@Override
            public SimpleDate getDueDate() { return dueDate; }

			@Override
            public EstimateType getType() { return type; }

			@Override
            public boolean isComplete() { return complete; }
		};
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
    public int getChildCount()
    {
		return children.size();
    }

	@Override
    public int indexOf(Estimate child)
    {
		return children.indexOf(child);
    }

	@Override
    public Estimate getChildAt(int index)
    {
		return children.get(index);
    }

	@Override
    public EstimateProgress getProgress(final ActualFigure actual,
    	final CurrencyCalculator calculator)
    {
		switch (type)
		{
			case INCOME:
				return new IncomeProgress(amount, actual.getAmount());
				
			case EXPENSE:
				return new ExpenseProgress(amount, actual.getAmount());
				
			case TRANSFER:
				return new TransferProgress(amount, actual.getAmount());
				
			case CATEGORY:
				return new CategoryProgress(calculator.zero(), calculator.zero());
				
			default:
				throw new IllegalArgumentException("Invalid estimate type");
		}
    }

	@Override
    public BalanceImpact getImpact(final ActualFigure actual,
    	final CurrencyCalculator calculator)
    {
		switch (type)
		{
			case INCOME:
				return new IncomeImpact(amount, actual.getAmount());
				
			case EXPENSE:
				return new ExpenseImpact(amount, actual.getAmount());
				
			case TRANSFER:
				return new TransferImpact(amount, actual.getAmount());
				
			case CATEGORY:
				return new CategoryImpact(amount, actual.getAmount());
				
			default:
				throw new IllegalArgumentException("Invalid estimate type");
		}
    }

	@Override
    public void setDefinition(EstimateDefinition definition)
    {
		name = definition.getName();
		description = definition.getDescription();
		amount = definition.getAmount();
		type = definition.getType();
		dueDate = (definition.getDueDate() == null) ? null
			: new XmlDate(definition.getDueDate());
		complete = definition.isComplete();
    }

	@Override
    public Estimate createChild()
    {
		EstimateDefinition definition = (children.size() == 0)
			? getDefinition()
			: getDefinition();
			
		XmlEstimate child = new XmlEstimate();
		child.setDefinition(definition);
		
		add(child);
		return child;
    }

	@Override
    public void delete()
    {
		parent.remove(this);
    }
	
	@Override
	public int hashCode()
	{
		int result = 2343;
		result = result * 31 + name.hashCode();
		result = result * 31 + ((description != null)
			? description.hashCode() : 0);
		result = result * 31 + ((amount != null)
			? amount.hashCode() : 0);
		result = result * 31 + ((type != null)
			? type.hashCode() : 0);
		result = result * 31 + ((dueDate != null)
			? dueDate.hashCode() : 0);
		result = result * 31 + (complete ? 1 : 0);
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof XmlEstimate))
			return false;
		
		XmlEstimate that = (XmlEstimate) obj;
		return this.name.equals(that.name)
			&& ((this.description == null) ? (that.description == null)
				: this.description.equals(that.description))
			&& ((this.amount == null) ? (that.amount == null)
				: this.amount.equals(that.amount))
			&& ((this.type == null) ? (that.type == null)
				: this.type.equals(that.type))
			&& ((this.dueDate == null) ? (that.dueDate == null)
				: this.dueDate.equals(that.dueDate))
			&& this.complete == that.complete;
	}
	
	@Override
	public String toString()
	{
		return name;
	}

}
