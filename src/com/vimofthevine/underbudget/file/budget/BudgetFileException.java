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

/**
 * Exception thrown when parsing or writing
 * to a budget file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFileException extends Exception {
	
	/**
	 * Constructs a new budget file exception with 
	 * the given message
	 * 
	 * @param msg exception detail message
	 */
	public BudgetFileException(String msg)
	{
		super(msg);
	}
	
	/**
	 * Constructs a new budget file exception with 
	 * the given message and cause
	 * 
	 * @param msg   exception detail message
	 * @param cuase exception cause
	 */
	public BudgetFileException(String msg, Throwable cause)
	{
		super(msg, cause);
	}

}
