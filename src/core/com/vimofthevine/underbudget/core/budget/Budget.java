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

package com.vimofthevine.underbudget.core.budget;

import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * A <code>budget</code> is the singular representation
 * of a financial budget (estimates) and how to
 * analyze the budget (assignment rules).
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Budget {
	
	/**
	 * Returns the definition of this budget.
	 * 
	 * @return budget definition
	 */
	public BudgetDefinition getDefinition();
	
	/**
	 * Returns the root estimate of the estimate tree.
	 * 
	 * @return estimate tree root
	 */
	public Estimate getRootEstimate();
	
	/**
	 * Returns the list of assignment rules.
	 * 
	 * @return assignment rules list
	 */
	public AssignmentRules getAssignmentRules();

}
