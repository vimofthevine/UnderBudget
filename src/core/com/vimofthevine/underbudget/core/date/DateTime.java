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

package com.vimofthevine.underbudget.core.date;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Default implementation of a <code>SimpleDate</code>,
 * based on the Java <code>java.util.Date</code>
 * class.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DateTime implements SimpleDate {
	
	private final Date date;
	
	/**
	 * Constructs a new date time with the
	 * current date/time.
	 */
	public DateTime()
	{
		date = new Date();
	}
	
	/**
	 * Constructs a new date time with the
	 * given <code>java.util.Date</code> object.
	 * 
	 * @param date date object
	 */
	public DateTime(Date date)
	{
		this.date = date;
	}
	
	@Override
	public int compareTo(SimpleDate that)
	{
		return this.date.compareTo(that.getTime());
	}

	@Override
	public boolean before(SimpleDate that)
	{
		return this.date.before(that.getTime());
	}

	@Override
	public boolean after(SimpleDate that)
	{
		return this.date.after(that.getTime());
	}

	@Override
	public String formatAsString()
	{
		SimpleDateFormat format = new SimpleDateFormat("EEE, MMM d, yyyy");
		return format.format(date);
	}

	@Override
	public Date getTime()
	{
		return date;
	}
	
	@Override
	public String toString()
	{
		return date.toString();
	}

}
