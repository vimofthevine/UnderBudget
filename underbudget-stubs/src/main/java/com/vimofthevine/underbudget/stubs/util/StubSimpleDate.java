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

package com.vimofthevine.underbudget.stubs.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubSimpleDate implements SimpleDate {
	
	private final Date date;
	
	public StubSimpleDate(int year, int month, int day)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.clear();
		calendar.set(Calendar.YEAR, year);
		calendar.set(Calendar.MONTH, month - 1);
		calendar.set(Calendar.DAY_OF_MONTH, day);
		date = calendar.getTime();
	}

	@Override
	public int compareTo(SimpleDate arg0)
	{
		return date.compareTo(arg0.getTime());
	}

	@Override
	public boolean before(SimpleDate when)
	{
		return date.before(when.getTime());
	}

	@Override
	public boolean after(SimpleDate when)
	{
		return date.after(when.getTime());
	}

	@Override
	public String formatAsString()
	{
		//DateFormat format = new SimpleDateFormat("EEE MMM d");
		DateFormat format = new SimpleDateFormat("MM/dd/yy");
		return format.format(date);
	}

	@Override
	public Date getTime()
	{
		return date;
	}

}
