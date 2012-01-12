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

package com.vimofthevine.underbudget.budget.file.parsers.meta;

import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;

/**
 * Interface for budget meta DOM parsers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetMetaDomParser {
	
	/**
	 * Extracts the budget meta info defined in the given
	 * DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return budget meta data defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public BudgetMeta parse(Document doc, float allocated)
	throws BudgetFileException;

}
