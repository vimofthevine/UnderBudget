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

package com.vimofthevine.underbudget.file.budget;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.progress.TaskProgress;

/**
 * Interface for a budget file
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetFile {

	/**
	 * Reads the budget file, extracting the budget
	 * defined in the file
	 * 
	 * @param progress read task progress
	 * @return budget defined in the file
	 * @throws BudgetFileException if any error occurs
	 */
	public Budget read(TaskProgress progress)
	throws BudgetFileException;
	
	/**
	 * Writes the given budget to the budget file
	 * 
	 * @param budget to be written to the file
	 * @param progress write task progress
	 * @throws BudgetFileException if any error occurs
	 */
	public void save(Budget budget, TaskProgress progress)
	throws BudgetFileException;
	
}
