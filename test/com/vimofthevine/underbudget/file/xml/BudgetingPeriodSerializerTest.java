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

package com.vimofthevine.underbudget.file.xml;

import static org.junit.Assert.*;

import java.io.StringWriter;

import org.junit.Test;
import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.period.CustomBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.YearlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the Simple xml framework
 * serializer used for budgeting periods
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetingPeriodSerializerTest {

	/**
	 * Verifies that custom budgeting periods are serialized correctly
	 */
	@Test
	public final void testCustomBudgetingPeriodSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"custom\">\n" +
			"   <start-date>\n" +
			"      <day>17</day>\n" +
			"      <month>2</month>\n" +
			"      <year>2012</year>\n" +
			"   </start-date>\n" +
			"   <end-date>\n" +
			"      <day>31</day>\n" +
			"      <month>2</month>\n" +
			"      <year>2012</year>\n" +
			"   </end-date>\n" +
			"</period>";
		final CustomBudgetingPeriod input = new CustomBudgetingPeriod(
			new SimpleDate(17, 2, 2012), new SimpleDate(31, 2, 2012));
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		StringWriter writer = new StringWriter();
		
		try
		{
			serializer.write(input, writer);
			assertEquals(expected, writer.toString());
		}
		catch (Exception e)
		{
			fail("Exception thrown during serialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that custom budgeting periods are deserialized correctly
	 */
	@Test
	public final void testCustomBudgetingPeriodDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"custom\">\n" +
			"   <start-date>\n" +
			"      <day>3</day>\n" +
			"      <month>4</month>\n" +
			"      <year>2012</year>\n" +
			"   </start-date>\n" +
			"   <end-date>\n" +
			"      <day>28</day>\n" +
			"      <month>4</month>\n" +
			"      <year>2012</year>\n" +
			"   </end-date>\n" +
			"</period>";
		final CustomBudgetingPeriod expected = new CustomBudgetingPeriod(
			new SimpleDate(3, 4, 2012), new SimpleDate(28, 4, 2012));
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			CustomBudgetingPeriod actual = serializer.read(CustomBudgetingPeriod.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that monthly budgeting periods are serialized correctly
	 */
	@Test
	public final void testMonthlyBudgetingPeriodSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"monthly\">\n" +
			"   <month>3</month>\n" +
			"   <year>2012</year>\n" +
			"</period>";
		final MonthlyBudgetingPeriod input = new MonthlyBudgetingPeriod(3, 2012);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		StringWriter writer = new StringWriter();
		
		try
		{
			serializer.write(input, writer);
			assertEquals(expected, writer.toString());
		}
		catch (Exception e)
		{
			fail("Exception thrown during serialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that monthly budgeting periods are deserialized correctly
	 */
	@Test
	public final void testMonthlyBudgetingPeriodDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"monthly\">\n" +
			"   <month>6</month>\n" +
			"   <year>2011</year>\n" +
			"</period>";
		final MonthlyBudgetingPeriod expected = new MonthlyBudgetingPeriod(6, 2011);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			MonthlyBudgetingPeriod actual = serializer.read(MonthlyBudgetingPeriod.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that yearly budgeting periods are serialized correctly
	 */
	@Test
	public final void testYearlyBudgetingPeriodSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"yearly\">\n" +
			"   <year>2012</year>\n" +
			"</period>";
		final YearlyBudgetingPeriod input = new YearlyBudgetingPeriod(2012);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		StringWriter writer = new StringWriter();
		
		try
		{
			serializer.write(input, writer);
			assertEquals(expected, writer.toString());
		}
		catch (Exception e)
		{
			fail("Exception thrown during serialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that yearly budgeting periods are deserialized correctly
	 */
	@Test
	public final void testYearlyBudgetingPeriodDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<period type=\"yearly\">\n" +
			"   <year>2011</year>\n" +
			"</period>";
		final YearlyBudgetingPeriod expected = new YearlyBudgetingPeriod(2011);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			YearlyBudgetingPeriod actual = serializer.read(YearlyBudgetingPeriod.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
}
