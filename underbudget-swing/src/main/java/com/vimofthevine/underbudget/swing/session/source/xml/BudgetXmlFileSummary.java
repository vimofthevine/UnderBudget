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

package com.vimofthevine.underbudget.swing.session.source.xml;

import java.io.File;

import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.source.SourceSummary;
import com.vimofthevine.underbudget.xml.budget.source.BudgetXmlFileSource;

/**
 * Minimum amount of information needed to be able to
 * re-open a budget XML file. No other information is
 * needed from the user.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetXmlFileSummary implements SourceSummary {
	
	/**
	 * Budget XML file
	 */
	private final File file;
	
	/**
	 * Constructs a definition of a recently opened
	 * budget XML file.
	 * 
	 * @param file budget XML file
	 */
	public BudgetXmlFileSummary(File budgetFile)
	{
		file = budgetFile;
	}

	@Override
	public String getName()
	{
		return file.getName();
	}
	
	@Override
	public boolean requiresPassword()
	{
		return false;
	}
	
	@Override
	public BudgetSource reopen(String passkey)
	{
		return new BudgetXmlFileSource(file);
	}

	@Override
	public void persist(int i, UserPreferences preferences)
	{
		// TODO Auto-generated method stub

	}
	
	@Override
	public int hashCode()
	{
		return 782 * 31 + file.hashCode();
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if ( ! (obj instanceof BudgetXmlFileSummary))
			return false;
		
		BudgetXmlFileSummary that = (BudgetXmlFileSummary) obj;
		return this.file.equals(that.file);
	}
	
	@Override
	public String toString()
	{
		return "Recent budget XML (" + file.getAbsolutePath() + ")";
	}

}
