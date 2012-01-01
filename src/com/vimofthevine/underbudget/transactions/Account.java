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
