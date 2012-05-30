package com.vimofthevine.underbudget.core.budget.estimate.behavior;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Stub DisplayableEstimate for unit testing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubDisplayableEstimate implements DisplayableEstimate {

	@Override
    public String getName()
    {
	    return "Stub Estimate";
    }

	@Override
    public String getDescription()
    {
	    return "Stub estimate for unit testing";
    }

	@Override
    public Currency getEstimatedAmount()
    {
		CurrencyFactory factory = new CurrencyFactory("USD");
	    return factory.newCurrencyInstance("123.45");
    }

	@Override
    public boolean moreTransactionsAreExpected()
    {
	    return true;
    }

}
