package com.vimofthevine.underbudget.budget.file.parsers;

import java.io.InputStream;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Interface for budget file parsers. It is not expected
 * that there would be multiple runtime-swappable file
 * parsers, but rather that in case the parsing algorithm
 * (i.e., the file format) changes in the future, the
 * parser can be easily swapped out.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetFileParser {
	
	/**
	 * Returns a reference to the task progress updater
	 * for the parser. This allows for progress listeners
	 * to register with the parser for progress updates.
	 * 
	 * @return task progress updater for the parser
	 */
	public TaskProgress getProgress();

	/**
	 * Parses the given input stream as a budget file
	 * 
	 * @param stream input stream from which to read
	 * @throws BudgetFileException if there are any errors with the file
	 */
	public void parse(InputStream stream) throws BudgetFileException;
	
	/**
	 * Returns the budget specified in the file
	 * 
	 * @return parsed budget from the file
	 */
	public Budget getBudget();
	
}
