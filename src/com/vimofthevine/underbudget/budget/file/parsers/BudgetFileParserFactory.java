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

package com.vimofthevine.underbudget.budget.file.parsers;

/**
 * Factory class for creating budget file parsers
 * and writers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class BudgetFileParserFactory {
	
	/**
	 * Create a new budget file parser
	 * 
	 * @return BudgetFileParser implementation
	 */
	public static BudgetFileParser createParser()
	{
		return new BudgetFileDomParser();
	}

}
