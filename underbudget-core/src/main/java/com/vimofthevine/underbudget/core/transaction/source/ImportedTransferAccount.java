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

package com.vimofthevine.underbudget.core.transaction.source;

import com.vimofthevine.underbudget.core.transaction.TransferAccount;

/**
 * Implementation of a <code>TransferAccount</code> used
 * for imported transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ImportedTransferAccount implements TransferAccount {
	
	/**
	 * Account name
	 */
	private final String name;
	
	/**
	 * Parent account
	 */
	private final TransferAccount parent;
	
	/**
	 * Account type
	 */
	private final String type;
	
	/**
	 * Constructs a new imported transfer account.
	 * 
	 * @param name   account name
	 * @param parent account parent
	 * @param type   account type
	 */
	public ImportedTransferAccount(String name,
		TransferAccount parent, String type)
	{
		this.name = name;
		this.parent = parent;
		this.type = type.toLowerCase();
	}
	
	/**
	 * Checks if this account represents an expense account.
	 * 
	 * @return <code>true</code> if this account is
	 *         an expense account
	 */
	public boolean isExpenseAccount()
	{
		return type.startsWith("exp");
	}

	@Override
	public String getName()
	{
		if (parent == null)
			return "";
		else
		{
			String parentName = parent.getName();
			if ( ! parentName.equals(""))
				return parentName + ":" + name;
			else
				return name;
		}
	}
	
	@Override
	public String toString()
	{
		return String.valueOf(parent) + ":" + name;
	}

}
