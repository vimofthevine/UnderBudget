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

package com.vimofthevine.underbudget.transactions;

/**
 * Representation of a financial account as imported
 * from a 3rd party source
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Account {
	
	/**
	 * Account ID
	 */
	public String id;
	
	/**
	 * Account name
	 */
	public String name;
	
	/**
	 * Account type
	 */
	public String type;
	
	/**
	 * Parent account
	 */
	public Account parent;
	
	/**
	 * Default constructor
	 */
	public Account()
	{
		
	}
	
	/**
	 * Class constructor
	 * 
	 * @param name account name
	 */
	public Account(String name)
	{
		this.name = name;
		this.type = "acct";
		this.parent = new Account("", "Root", "root", null);
	}
	
	/**
	 * Class constructor
	 * 
	 * @param id     account ID
	 * @param name   account name
	 * @param type   account type
	 * @param parent parent account
	 */
	public Account(String id, String name, String type, Account parent)
	{
		this.id     = id;
		this.name   = name;
		this.type   = type;
		this.parent = parent;
	}
	
	/**
	 * Returns the fully qualified name of this account
	 * 
	 * @return fully qualified name of this account
	 */
	public String getName()
	{
		if (type.equalsIgnoreCase("root") || parent == null)
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

}
