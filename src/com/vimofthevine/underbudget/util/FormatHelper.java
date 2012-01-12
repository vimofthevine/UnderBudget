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
	
	/**
	 * Truncates a given string to the given number
	 * of characters, if the string length exceeds
	 * the given length. If the string length is
	 * less than the number of characters specified,
	 * the remaining space is padded with spaces.
	 * 
	 * @param string string to truncate
	 * @param length number of characters allowed
	 */
	public static String truncate(String string, int length)
	{
		if (string.length() > length)
			return string.substring(0, length);
		else
		{
			StringBuffer buffer = new StringBuffer(string);
			
			for (int i=0; i<(length - string.length()); i++)
			{
				buffer.append(" ");
			}
			
			return buffer.toString();
		}
	}

}
