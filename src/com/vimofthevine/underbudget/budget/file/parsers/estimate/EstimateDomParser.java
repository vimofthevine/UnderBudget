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

package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Interface for estimate DOM parsers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateDomParser {
	
	/**
	 * Extracts the income estimate definitions defined
	 * in the given DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return income estimate root as defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public Estimate parseIncomes(Document doc, float allocated)
	throws BudgetFileException;
	
	/**
	 * Extracts the expense estimate definitions defined
	 * in the given DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return expense estimate root as defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public Estimate parseExpenses(Document doc, float allocated)
	throws BudgetFileException;

}
