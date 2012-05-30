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

package com.vimofthevine.underbudget.core.ledger;

import org.simpleframework.xml.Text;

/**
 * Representation of a financial account as imported
 * from a 3rd party source.
 * 
 * This is currently based on GnuCash. If there is some more
 * universal way of determine what kind of account is
 * being represented, then the is* methods should be
 * updated.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Account {
	
	/**
	 * Account name. The only reason
	 * this field is not final is because
	 * it needs to be set by setName when
	 * deserializing.
	 */
	private String name;
	
	/**
	 * Parent account
	 */
	private Account parent;
	
	/**
	 * Private null-parameter constructor
	 */
	private Account()
	{
		parent = null;
	}
	
	/**
	 * Class constructor
	 * 
	 * @param name account name
	 */
	public Account(String name)
	{
		this.name = name;
		this.parent = new Account("Root", null);
	}
	
	/**
	 * Class constructor
	 * 
	 * @param name   account name
	 * @param parent parent account
	 */
	public Account(String name, Account parent)
	{
		this.name   = name;
		this.parent = parent;
	}
	
	/**
	 * Returns the fully qualified name of this account
	 * 
	 * @return fully qualified name of this account
	 */
	@Text
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
	
	@Text
	private void setName(String name)
	{
		this.name = name;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof Account)
		{
			Account that = (Account) obj;
			
			return this.name.equals(that.name);
		}
		else
			return super.equals(obj);
	}

}
