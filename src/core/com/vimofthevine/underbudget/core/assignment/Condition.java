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

package com.vimofthevine.underbudget.core.assignment;

/**
 * An individual condition that must be satisifed
 * for a transaction to match an assignment rule.
 * An assignment rule may have multiple conditions,
 * for which all must be satisfied.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Condition {
	
	/**
	 * Returns the evaluation transaction field of this condition.
	 * 
	 * @return evaluation transaction field
	 */
	public TransactionField getField();
	
	/**
	 * Returns the evaluation operator of this condition.
	 * 
	 * @return evaluation operator
	 */
	public EvaluationOperator getOperator();
	
	/**
	 * Returns the evaluation value of this condition.
	 * 
	 * @return evaluation value
	 */
	public String getValue();
	
	/**
	 * Checks if the given value satisfies the
	 * criteria of this condition.
	 * 
	 * @param value value to be checked
	 * @return <code>true</code> if the value
	 *         meets the criteria of this condition,
	 *         else <code>false</code>
	 */
	public boolean satisfies(String value);

}
