package com.vimofthevine.underbudget.util;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Convenience class providing display formatting
 * utilities
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
abstract public class FormatHelper {
	
	/**
	 * Formats a currency value to the appropriate
	 * display format. Values are shown with two
	 * decimal places and a dollar sign.
	 * 
	 * @param value currency value to format
	 * @return formatted currency value
	 */
	public static String formatCurrency(BigDecimal value)
	{
		DecimalFormat format = new DecimalFormat("$#.#");
		format.setMinimumFractionDigits(2);
		format.setMaximumFractionDigits(2);
		format.setGroupingUsed(true);
		format.setGroupingSize(3);
		return format.format(value);
	}
	
	/**
	 * Formats a date value to the appropriate
	 * display format. Values are shown as
	 * <month>/<date>/<year>.
	 * 
	 * @param date date value to format
	 * @return formatted date value
	 */
	public static String formatDate(Date date)
	{
		SimpleDateFormat format = new SimpleDateFormat("M/d/yy");
		return format.format(date);
	}

}
