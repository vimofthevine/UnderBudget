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
 * Enumeration for the fields of a transaction
 * that can be evaluated for condition-matching criteria.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum TransactionField {
	NONE("None"),
	MEMO("Memo"),
	PAYEE("Payee"),
	WITHDRAWAL("Withdrawal Account"),
	DEPOSIT("Deposit Account");
	
	private final String name;
	
	private TransactionField(String name)
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
	 * @param string string representation of a field
	 * @return transaction field enumeration
	 */
	public static TransactionField getValueOf(String string)
	{
		string = string.toLowerCase();
		
		if (string.contains("memo"))
			return MEMO;
		else if (string.contains("payee"))
			return PAYEE;
		else if (string.contains("withdrawal"))
			return WITHDRAWAL;
		else if (string.contains("deposit"))
			return DEPOSIT;
		else
			return NONE;
	}
	
}
