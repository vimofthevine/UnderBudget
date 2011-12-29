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
