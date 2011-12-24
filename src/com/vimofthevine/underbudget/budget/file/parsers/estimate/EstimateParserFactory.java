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
	 * Creates a new estimate parser, according
	 * to the given version of the budget file
	 * being read
	 * 
	 * @param version  budget file version
	 * @param progress task progress
	 * @return estimate parser
	 */
	public static EstimateDomParser createParser(int version, TaskProgress progress)
	{
		if (version == 1)
			return new EstimateDomParserV1(progress);
		else if (version == 2)
			return new EstimateDomParserV2(progress);
		else
			return new EstimateDomParserV3(progress);
	}
	
}
