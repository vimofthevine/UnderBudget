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

package com.vimofthevine.underbudget.report;

import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * An entry for the worksheet report, indicating the
 * effect of each estimate on the balance totals
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class WorksheetEntry {
	
	/**
	 * The estimate for this entry
	 */
	public Estimate estimate;
	
	/**
	 * The balance totals for this entry
	 */
	public BalanceTotal totals;
	
	/**
	 * The rationale for the use of estimated vs. actual
	 */
	public String rationale;
	
	/**
	 * Constructor
	 * 
	 * @param estimate  entry estimate
	 * @param totals    entry totals
	 * @param rationale effective amount rationale
	 */
	public WorksheetEntry(Estimate estimate, BalanceTotal totals, String rationale)
	{
		this.estimate  = estimate;
		this.totals    = totals.clone();
		this.rationale = rationale;
	}

}
