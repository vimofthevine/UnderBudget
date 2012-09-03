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
 * Enumeration for transaction field evaluation operations
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum EvaluationOperator {
	NONE("None"),
	BEGINS_WITH("Begins With"),
	EQUALS("Equals (case insensitive)"),
	EQUALS_CASE("Equals (case sensitive)"),
	CONTAINS("Contains"),
	ENDS_WITH("Ends With");
	
	private final String name;
	
	private EvaluationOperator(String name)
	{
		this.name = name;
	}
	
	@Override
	public String toString()
	{
		return name;
	}
	
	/**
	 * Converts a given string to the
	 * appropriate corresponding enumeration
	 * 
	 * @param string string representation of an operator
	 * @return comparison operator enumeration
	 */
	public static EvaluationOperator getValueOf(String string)
	{
		string = string.toLowerCase();
		
		if (string.contains("begin"))
			return BEGINS_WITH;
		else if (string.contains("case"))
			return EQUALS_CASE;
		else if (string.contains("equal"))
			return EQUALS;
		else if (string.contains("contain"))
			return CONTAINS;
		else if (string.contains("end"))
			return ENDS_WITH;
		else
			return NONE;
	}
	
}
