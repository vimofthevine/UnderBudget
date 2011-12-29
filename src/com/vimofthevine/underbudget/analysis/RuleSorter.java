package com.vimofthevine.underbudget.analysis;

import java.util.ArrayList;
import java.util.List;

import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.UnbudgetedExpense;
import com.vimofthevine.underbudget.estimates.UnbudgetedIncome;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.estimates.rules.UnbudgetedExpenseRule;
import com.vimofthevine.underbudget.estimates.rules.UnbudgetedIncomeRule;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Sorter for prioritizing estimate rules, according
 * to the rule types
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleSorter {
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Task progress allocation
	 */
	protected float allocated;
	
	/**
	 * Rules with the equals-case (case sensitive) operator
	 */
	ArrayList<EstimateRule> equalsCaseList;
	
	/**
	 * Rules with the equals (case insensitive) operator
	 */
	ArrayList<EstimateRule> equalsList;
	
	/**
	 * Rules with the begins-with operator
	 */
	ArrayList<EstimateRule> beginsWithList;
	
	/**
	 * Rules with the ends-with operator
	 */
	ArrayList<EstimateRule> endsWithList;
	
	/**
	 * Rules with the contains operator
	 */
	ArrayList<EstimateRule> containsList;
	
	/**
	 * Class constructor
	 * 
	 * @param progress  task progress updater
	 * @param allocated percentage of the total task allocated
	 *                  to this task
	 */
	public RuleSorter(TaskProgress progress, float allocated)
	{
		this.progress = progress;
		this.allocated = allocated;
	}
	
	/**
	 * Sorts the estimate rules according to their
	 * operator type.
	 * 
	 * - 
	 * 
	 * @param income  root income estimate
	 * @param expense root expense estimate
	 */
	public List<EstimateRule> sort(Estimate income, Estimate expense)
	throws BudgetAnalysisException
	{
		// Set up lists for each operator type
		equalsCaseList = new ArrayList<EstimateRule>();
		equalsList     = new ArrayList<EstimateRule>();
		beginsWithList = new ArrayList<EstimateRule>();
		endsWithList   = new ArrayList<EstimateRule>();
		containsList   = new ArrayList<EstimateRule>();
		
		// Sort the estimate rules
		sortRulesForEstimate(income, allocated / 2);
		sortRulesForEstimate(expense, allocated / 2);
		
		// Combine lists
		ArrayList<EstimateRule> rules = new ArrayList<EstimateRule>();
		rules.addAll(equalsCaseList);
		rules.addAll(equalsList);
		rules.addAll(beginsWithList);
		rules.addAll(endsWithList);
		rules.addAll(containsList);
		
		// Add unbudgeted/catch-alls at the end
		rules.add(new EstimateRule(new UnbudgetedIncome(), new UnbudgetedIncomeRule()));
		rules.add(new EstimateRule(new UnbudgetedExpense(), new UnbudgetedExpenseRule()));
		
		return rules;
	}
	
	/**
	 * Recursively sorts the rules for the given estimate
	 * into one of the operator rule maps, descending into
	 * the estimate sub-tree if applicable
	 * 
	 * @param estimate  estimate whose rules are to be sorted
	 * @param allocated percentage of the overall task allocated
	 *                  to this particular task
	 */
	protected void sortRulesForEstimate(Estimate estimate, float allocated)
	{
		// Descend into estimate sub-tree
		if (estimate.isCategory())
		{
			float portion = allocated / (float) estimate.getChildCount();
			
			for (int i=0; i<estimate.getChildCount(); i++)
			{
				sortRulesForEstimate(estimate.getChildAt(i), portion);
			}
		}
		// Else sort estimate's rules
		else
		{
			for (Rule rule : estimate.getRules())
			{
				switch (rule.getOperator())
				{
					case EQUALS_CASE:
						addToList(equalsCaseList, estimate, rule);
						break;
					case EQUALS:
						addToList(equalsList, estimate, rule);
						break;
					case BEGINS_WITH:
						addToList(beginsWithList, estimate, rule);
						break;
					case ENDS_WITH:
						addToList(endsWithList, estimate, rule);
						break;
					case CONTAINS:
						addToList(containsList, estimate, rule);
						break;
				}
			}
			
			progress.add(allocated);
		}
	}
	
	/**
	 * Adds the estimate-rule combination to the given rule list
	 * 
	 * @param list     rule list to which the combo will be added
	 * @param estimate estimate to add
	 * @param rule     rule to add
	 */
	protected void addToList(List<EstimateRule> list, Estimate estimate, Rule rule)
	{
		// Add any-field rules to the end of the list
		if (rule.getField().equals(TransactionField.ANY))
		{
			list.add(new EstimateRule(estimate, rule));
		}
		// Add all others to the front of the list
		else
		{
			list.add(0, new EstimateRule(estimate, rule));
		}
	}
	
}
