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

package com.vimofthevine.underbudget.core.budget.source;

/**
 * Exception thrown if any error occurs and a budget
 * could not be retrieved from its source or persisted
 * at the source.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSourceException extends Exception {
	
	/**
	 * Constructs a new budget source exception with 
	 * the given message
	 * 
	 * @param msg exception detail message
	 */
	public BudgetSourceException(String msg)
	{
		super(msg);
	}
	
	/**
	 * Constructs a new budget source exception with 
	 * the given message and cause
	 * 
	 * @param msg   exception detail message
	 * @param cuase exception cause
	 */
	public BudgetSourceException(String msg, Throwable cause)
	{
		super(msg, cause);
	}

}
