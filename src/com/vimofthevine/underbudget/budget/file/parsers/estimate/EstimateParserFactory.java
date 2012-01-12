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
