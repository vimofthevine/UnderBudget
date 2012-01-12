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

package com.vimofthevine.underbudget.analysis;

import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.Rule;

/**
 * Representation of an estimate-rule combination
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateRule {
	
	/**
	 * Estimate to which the rule belongs
	 */
	public Estimate estimate;
	
	/**
	 * Rule for matching against the estimate
	 */
	public Rule rule;
	
	/**
	 * Class constructor
	 * 
	 * @param estimate estimate to which the rule belongs
	 * @param rule     rule for matching against the estimate
	 */
	public EstimateRule(Estimate estimate, Rule rule)
	{
		this.estimate = estimate;
		this.rule     = rule;
	}

}
