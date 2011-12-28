package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Factory class for creating estimate parsers,
 * according to the version of the budget file
 * being read
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class EstimateParserFactory {
	
	/**
	 * Stub EstimateDomParser to use for unit testing
	 */
	public static EstimateDomParser stubDomParser = null;

	/**
	 * Creates a new estimate DOM parser, according
	 * to the given version of the budget file
	 * being read
	 * 
	 * @param version  budget file version
	 * @param progress task progress
	 * @return estimate DOM parser
	 */
	public static EstimateDomParser createDomParser(int version, TaskProgress progress)
	{
		if (stubDomParser != null)
			return stubDomParser;
		
		if (version == 1)
			return new EstimateDomParserV1(progress);
		else if (version == 2)
			return new EstimateDomParserV2(progress);
		else
			return new EstimateDomParserV3(progress);
	}
	
}
