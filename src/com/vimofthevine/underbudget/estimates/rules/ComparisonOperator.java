package com.vimofthevine.underbudget.estimates.rules;

/**
 * Enumeration for text comparison operations
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum ComparisonOperator {
	NONE,
	BEGINS_WITH,
	EQUALS,
	EQUALS_CASE,
	CONTAINS,
	ENDS_WITH;
	
	/**
	 * Returns an array of values, suitable for
	 * a user-selection list
	 * 
	 * @return user-selection values
	 */
	public static String[] userValues()
	{
		return new String[] {
				"Begins With", "Equals",
				"Equals (case sensitive)", "Contains",
				"Ends With"
		};
	}
	
}
