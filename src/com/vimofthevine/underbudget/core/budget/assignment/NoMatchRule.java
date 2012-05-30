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

package com.vimofthevine.underbudget.core.budget.assignment;

import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;

/**
 * A <code>NoMatchRule</code> is a special <code>AssignmentRule</code>
 * used to catch transactions for which there is no user assignment rule
 * with matching criteria.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class NoMatchRule implements AssignmentRule {
	
	/**
	 * Accepts the given transaction as an ignored
	 * transaction. That is, there are no user
	 * assignment rules that the transaction matched.
	 */
	@Override
	public boolean matches(AnalyzableTransaction transaction)
	{
		return true;
	}
	
	@Override
	public String formatAsString()
	{
		return "Ignored, no match found";
	}
	
	@Override
	public boolean equals(Object obj)
	{
		return (obj instanceof NoMatchRule);
	}
	
	@Override
	public int hashCode()
	{
		return 32;
	}

}
