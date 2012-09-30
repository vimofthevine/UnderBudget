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

package com.vimofthevine.underbudget.xml.budget.source;

import java.io.File;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceFactory;

/**
 * Factory for creating budget XML file sources.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetXmlFileFactory implements BudgetSourceFactory {
	
	/**
	 * Budget XML file
	 */
	private final File file;
	
	/**
	 * Constructs a new budget XML file source factory.
	 * 
	 * @param file budget XML file
	 */
	public BudgetXmlFileFactory(File file)
	{
		this.file = file;
	}

	@Override
	public BudgetSource create()
	{
		return new BudgetXmlFileSource(file);
	}

	@Override
	public BudgetSource create(Budget budget)
	{
		return new BudgetXmlFileSource(file, budget);
	}

}
