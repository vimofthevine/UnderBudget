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

package com.vimofthevine.underbudget.swing.assignment;

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.RuleDefinition;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class UserAssignmentRule implements AssignmentRule {
	
	/**
	 * Associated estimate
	 */
	private final Estimate estimate;
	
	UserAssignmentRule(Estimate estimate)
	{
		this.estimate = estimate;
	}

	@Override
	public RuleDefinition getDefinition()
	{
		return new RuleDefinition() {
			@Override
            public Condition[] getConditions()
            {
				return new Condition[0];
            }
		};
	}

	@Override
	public boolean matches(Transaction transaction)
	{
		return false;
	}

	@Override
	public Estimate getEstimate()
	{
		return estimate;
	}

}
