package com.vimofthevine.underbudget.budget;

import java.math.BigDecimal;
import java.util.Observable;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;

/**
 * Representation of user-defined budget meta data
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMeta extends Observable {

	/**
	 * The name of the budget
	 */
	private String name;
	
	/**
	 * The period the budget covers
	 */
	private BudgetingPeriod period;
	
	/**
	 * The initial financial balance prior to the
	 * budgeting period
	 */
	private BigDecimal initial;
	
	/**
	 * Default constructor
	 */
	public BudgetMeta()
	{
		name    = "";
		period  = new MonthlyBudgetingPeriod();
		initial = new BigDecimal("0");
	}
	
	/**
	 * Constructor, specifying the budget name,
	 * budgeting period, and initial balance
	 * 
	 * @param name    budget name
	 * @param period  budgeting period
	 * @param initial initial balance
	 */
	public BudgetMeta(String name, BudgetingPeriod period, BigDecimal initial)
	{
		this.name    = name;
		this.period  = period;
		this.initial = initial;
	}
	
	/**
	 * Returns the name of the budget
	 *  
	 * @return budget name
	 */
	public String getName()
	{
		return name;
	}

	/**
	 * Sets the name of the budget. If the new
	 * name is different than the current name,
	 * the meta data is marked as changed.
	 * 
	 * @param name budget name
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
	 * Returns the budgeting period covered
	 * by the budget
	 * 
	 * @return period covered by the budget
	 */
	public BudgetingPeriod getPeriod()
	{
		return period;
	}
	
	/**
	 * Defines the budgeting period for which
	 * the budget covers. If the new period is
	 * different than the current period, the
	 * meta data is marked as changed.
	 * 
	 * @param period period covered by the budget
	 */
	public void setPeriod(BudgetingPeriod period)
	{
		if ( ! this.period.equals(period))
		{
			this.period = period;
			setChanged();
		}
	}
	
	/**
	 * Returns the user-defined initial balance
	 * prior to the budgeting period start
	 * 
	 * @return initial balance prior to
	 *         the budgeting period start
	 */
	public BigDecimal getInitialBalance()
	{
		return initial;
	}
	
	/**
	 * Sets the initial balance prior to the
	 * budgeting period start. If the new initial 
	 * balance is different than the current
	 * balance, the meta data is marked as changed.
	 * 
	 * @param balance initial balance prior to
	 *                the budgeting period start
	 */
	public void setInitialBalance(BigDecimal balance)
	{
		if ( ! initial.equals(balance))
		{
			initial = balance;
			setChanged();
		}
	}
	
	/**
	 * Returns the budget name as the string
	 * representation of this budget meta
	 * 
	 * @return string representation of this estimate
	 */
	@Override
	public String toString()
	{
		return name;
	}
	
}
