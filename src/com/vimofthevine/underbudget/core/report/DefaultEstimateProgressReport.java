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

package com.vimofthevine.underbudget.core.report;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Default estimate progress report
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultEstimateProgressReport implements EstimateProgressReport {

	@Override
    public void recordDueDate(DisplayableEstimate estimate, SimpleDate due)
    {
		// Do nothing
    }

	@Override
    public void recordOverBudget(DisplayableEstimate estimate, Currency estimated,
            Currency actual)
    {
		// Do nothing
    }

	@Override
    public void recordUnderBudget(DisplayableEstimate estimate, Currency estimated,
            Currency actual)
    {
		// Do nothing
    }

	@Override
    public void recordUnbudgeted(DisplayableEstimate estimate, Currency actual)
    {
		// Do nothing
    }

	@Override
    public void recordProgress(DisplayableEstimate estimate, Currency estimated,
            Currency actual)
    {
		// Do nothing
    }

}
