package com.vimofthevine.underbudget.core.budget.estimate.behavior;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Stub EstimateProgressReport for unit testing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubEstimateProgressReport implements EstimateProgressReport {
	
	/**
	 * Recorded due date
	 */
	public SimpleDate dueDate;
	
	/**
	 * Recorded over-budget
	 */
	public boolean overBudget = false;
	
	/**
	 * Recorded under-budget
	 */
	public boolean underBudget = false;
	
	/**
	 * Recorded unbudgeted
	 */
	public boolean unbudgeted = false;
	
	/**
	 * Recorded progress
	 */
	public boolean progress = false;

	@Override
    public void recordDueDate(DisplayableEstimate estimate, SimpleDate due)
    {
		dueDate = due;
    }

	@Override
    public void recordOverBudget(DisplayableEstimate estimate,
            Currency estimated, Currency actual)
    {
		overBudget = true;
    }

	@Override
    public void recordUnderBudget(DisplayableEstimate estimate,
            Currency estimated, Currency actual)
    {
		underBudget = true;
    }

	@Override
    public void recordUnbudgeted(DisplayableEstimate estimate, Currency actual)
    {
		unbudgeted = true;
    }

	@Override
    public void recordProgress(DisplayableEstimate estimate,
            Currency estimated, Currency actual)
    {
	    // TODO Auto-generated method stub
	    
    }

}
