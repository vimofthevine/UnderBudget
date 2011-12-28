package com.vimofthevine.underbudget.budget.file.parsers.meta;

import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Factory class for creating budget meta parsers,
 * according to the version of the budget file
 * being read
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class BudgetMetaParserFactory {
	
	/**
	 * Stub BudgetMetaDomParser to use for unit testing
	 */
	public static BudgetMetaDomParser stubDomParser = null;

	/**
	 * Creates a new budget meta DOM parser,
	 * according to the given version of the
	 * budget file being read
	 * 
	 * @param version  budget file version
	 * @param progress task progress
	 * @return budget meta DOM parser
	 */
	public static BudgetMetaDomParser createDomParser(int version, TaskProgress progress)
	{
		if (stubDomParser != null)
			return stubDomParser;
		
		if (version < 3)
			return new BudgetMetaDomParserV1(progress);
		else
			return new BudgetMetaDomParserV3(progress);
	}
	
}
