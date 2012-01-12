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

package com.vimofthevine.underbudget.analysis;

import java.util.List;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.report.WorksheetEntry;

/**
 * Representation of all analysis artifacts
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AnalysisResults {
	
	/**
	 * Budget that has been analyzed
	 */
	public Budget budget;
	
	/**
	 * List of sorted estimate rules
	 */
	public List<EstimateRule> rules;
	
	/**
	 * List of allocation entries
	 */
	public List<AllocationEntry> allocation;
	
	/**
	 * List of worksheet entries
	 */
	public List<WorksheetEntry> worksheet;
	
	/**
	 * Balance totals
	 */
	public BalanceTotal total;

}
