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

import com.vimofthevine.underbudget.core.budget.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.GenericEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.OwedEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.SpecificEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.UnbudgetedEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.impact.ExpenseImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.IncomeImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.NoImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.TransferImpact;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the Simple xml framework
 * serializer used for estimate definitions
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateSerializerTest {
	
	/**
	 * Currency factory
	 */
	private final static CurrencyFactory currency = new CurrencyFactory("USD");
	
	/**
	 * Estimate tree XML
	 */
	private final static String XML = BudgetSerializerFactory.PROLOG + "\n" +
		"<estimate id=\"0\">\n" +
		"   <name>Level 1</name>\n" +
		"   <behavior type=\"generic\"/>\n" +
		"   <estimates>\n" +
		"      <estimate id=\"1\">\n" +
		"         <name>Level 2</name>\n" +
		"         <behavior type=\"generic\"/>\n" +
		"         <estimates>\n" +
		"            <estimate id=\"2\">\n" +
		"               <name>Level 3a</name>\n" +
		"               <impact>income</impact>\n" +
		"               <description>A low-level estimate</description>\n" +
		"               <amount currency=\"USD\">10.50</amount>\n" +
		"               <behavior type=\"owed\">\n" +
		"                  <due>\n" +
		"                     <day>15</day>\n" +
		"                     <month>3</month>\n" +
		"                     <year>2012</year>\n" +
		"                  </due>\n" +
		"               </behavior>\n" +
		"            </estimate>\n" +
		"            <estimate id=\"3\">\n" +
		"               <name>Level 3b</name>\n" +
		"               <impact>expense</impact>\n" +
		"               <amount currency=\"USD\">100.00</amount>\n" +
		"               <final/>\n" +
		"               <behavior type=\"generic\"/>\n" +
		"            </estimate>\n" +
		"         </estimates>\n" +
		"      </estimate>\n" +
		"   </estimates>\n" +
		"</estimate>";

	/**
	 * Verifies that generic estimates are serialized correctly
	 */
	@Test
	public final void testGenericEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Food</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">50.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Food", "", currency.newCurrencyInstance("50.00"),
			false, new GenericEstimateBehavior(), new ExpenseImpact()
		);
		
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
	 * Verifies that generic estimates are deserialized correctly
	 */
	@Test
	public final void testGenericEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Food</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">50.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Food", "", currency.newCurrencyInstance("50.00"),
			false, new GenericEstimateBehavior(), new ExpenseImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that owed estimates are serialized correctly
	 */
	@Test
	public final void testOwedEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Taxes</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <description>Federal taxes</description>\n" +
			"   <amount currency=\"USD\">10.50</amount>\n" +
			"   <behavior type=\"owed\">\n" +
			"      <due>\n" +
			"         <day>15</day>\n" +
			"         <month>3</month>\n" +
			"         <year>2012</year>\n" +
			"      </due>\n" +
			"   </behavior>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Taxes", "Federal taxes", currency.newCurrencyInstance("10.50"),
			false, new OwedEstimateBehavior(new SimpleDate(15,3,2012)), new ExpenseImpact()
		);
		
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
	 * Verifies that owed estimates are deserialized correctly
	 */
	@Test
	public final void testOwedEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Taxes</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <description>Federal Taxes</description>\n" +
			"   <amount currency=\"USD\">10.50</amount>\n" +
			"   <behavior type=\"owed\">\n" +
			"      <due>\n" +
			"         <day>15</day>\n" +
			"         <month>3</month>\n" +
			"         <year>2012</year>\n" +
			"      </due>\n" +
			"   </behavior>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Taxes", "Federal Taxes", currency.newCurrencyInstance("10.50"),
			false, new OwedEstimateBehavior(new SimpleDate(15,3,2012)), new ExpenseImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that specific estimates are serialized correctly
	 */
	@Test
	public final void testSpecificEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Gift</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <description>Birthday gift</description>\n" +
			"   <amount currency=\"USD\">25.50</amount>\n" +
			"   <behavior type=\"specific\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Gift", "Birthday gift", currency.newCurrencyInstance("25.50"),
			false, new SpecificEstimateBehavior(), new ExpenseImpact()
		);
		
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
	 * Verifies that specific estimates are deserialized correctly
	 */
	@Test
	public final void testSpecificEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Gift</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <description>Birthday gift</description>\n" +
			"   <amount currency=\"USD\">25.50</amount>\n" +
			"   <behavior type=\"specific\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Gift", "Birthday gift", currency.newCurrencyInstance("25.50"),
			false, new SpecificEstimateBehavior(), new ExpenseImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that unbudgeted estimates are serialized correctly
	 */
	@Test
	public final void testUnbudgetedEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Unbudgeted Expenses</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">0.00</amount>\n" +
			"   <behavior type=\"unbudgeted\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Unbudgeted Expenses", "", currency.newCurrencyInstance("0.00"),
			false, new UnbudgetedEstimateBehavior(), new ExpenseImpact()
		);
		
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
	 * Verifies that unbudgeted estimates are deserialized correctly
	 */
	@Test
	public final void testUnbudgetedEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Unbudgeted Expenses</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">0.00</amount>\n" +
			"   <behavior type=\"unbudgeted\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Unbudgeted Expenses", "", currency.newCurrencyInstance("0.00"),
			false, new UnbudgetedEstimateBehavior(), new ExpenseImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that income estimates are serialized correctly
	 */
	@Test
	public final void testIncomeEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Salary</name>\n" +
			"   <impact>income</impact>\n" +
			"   <amount currency=\"USD\">500.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Salary", "", currency.newCurrencyInstance("500.00"),
			false, new GenericEstimateBehavior(), new IncomeImpact()
		);
		
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
	 * Verifies that income estimates are deserialized correctly
	 */
	@Test
	public final void testIncomeEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Salary</name>\n" +
			"   <impact>income</impact>\n" +
			"   <amount currency=\"USD\">500.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Salary", "", currency.newCurrencyInstance("500.00"),
			false, new GenericEstimateBehavior(), new IncomeImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that expense estimates are serialized correctly
	 */
	@Test
	public final void testExpenseEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Utilities</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">200.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Utilities", "", currency.newCurrencyInstance("200.00"),
			false, new GenericEstimateBehavior(), new ExpenseImpact()
		);
		
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
	 * Verifies that expense estimates are deserialized correctly
	 */
	@Test
	public final void testExpenseEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Utilities</name>\n" +
			"   <impact>expense</impact>\n" +
			"   <amount currency=\"USD\">200.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Utilities", "", currency.newCurrencyInstance("200.00"),
			false, new GenericEstimateBehavior(), new ExpenseImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that transfer estimates are serialized correctly
	 */
	@Test
	public final void testTransferEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Payment</name>\n" +
			"   <impact>transfer</impact>\n" +
			"   <amount currency=\"USD\">100.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Payment", "", currency.newCurrencyInstance("100.00"),
			false, new GenericEstimateBehavior(), new TransferImpact()
		);
		
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
	 * Verifies that transfer estimates are deserialized correctly
	 */
	@Test
	public final void testTransferEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Payment</name>\n" +
			"   <impact>transfer</impact>\n" +
			"   <amount currency=\"USD\">100.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Payment", "", currency.newCurrencyInstance("100.00"),
			false, new GenericEstimateBehavior(), new TransferImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that no-impact estimates are serialized correctly
	 */
	@Test
	public final void testNoImpactEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Category</name>\n" +
			"   <amount currency=\"USD\">0.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Category", "", currency.newCurrencyInstance("0.00"),
			false, new GenericEstimateBehavior(), new NoImpact()
		);
		
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
	 * Verifies that no-impact estimates are deserialized correctly
	 */
	@Test
	public final void testNoImpactEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Category</name>\n" +
			"   <amount currency=\"USD\">0.00</amount>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Category", "", currency.newCurrencyInstance("0.00"),
			false, new GenericEstimateBehavior(), new NoImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that final estimates are serialized correctly
	 */
	@Test
	public final void testFinalEstimateSerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Complete</name>\n" +
			"   <amount currency=\"USD\">5.00</amount>\n" +
			"   <final/>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate input = new DefaultEstimate(
			"Complete", "", currency.newCurrencyInstance("5.00"),
			true, new GenericEstimateBehavior(), new NoImpact()
		);
		
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
	 * Verifies that final estimates are deserialized correctly
	 */
	@Test
	public final void testFinalEstimateDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<estimate id=\"0\">\n" +
			"   <name>Complete</name>\n" +
			"   <amount currency=\"USD\">5.00</amount>\n" +
			"   <final/>\n" +
			"   <behavior type=\"generic\"/>\n" +
			"</estimate>";
		final DefaultEstimate expected = new DefaultEstimate(
			"Complete", "", currency.newCurrencyInstance("5.00"),
			true, new GenericEstimateBehavior(), new NoImpact()
		);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
	
	/**
	 * Verifies that estimate trees are serialized correctly
	 */
	@Test
	public final void testEstimateTreeSerialization()
	{
		final String expected = XML;
		DefaultEstimate level3a = new DefaultEstimate(
			"Level 3a", "A low-level estimate", currency.newCurrencyInstance("10.50"),
			false, new OwedEstimateBehavior(new SimpleDate(15, 3, 2012)), new IncomeImpact());
		DefaultEstimate level3b = new DefaultEstimate(
			"Level 3b", "", currency.newCurrencyInstance("100.00"),
			true, new GenericEstimateBehavior(), new ExpenseImpact());
		DefaultEstimate level2 = new DefaultEstimate(
			"Level 2", "", null,
			false, new GenericEstimateBehavior(), new NoImpact());
		level2.addChild(level3a);
		level2.addChild(level3b);
		final DefaultEstimate input = new DefaultEstimate(
			"Level 1", "", null,
			false, new GenericEstimateBehavior(), new NoImpact());
		input.addChild(level2);
		
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
	 * Verifies that estimate trees are deserialized correctly
	 */
	@Test
	public final void testEstimateTreeDeserialization()
	{
		final String input = XML;
		DefaultEstimate level3a = new DefaultEstimate(
			"Level 3a", "A low-level estimate", currency.newCurrencyInstance("10.50"),
			false, new OwedEstimateBehavior(new SimpleDate(15, 3, 2012)), new IncomeImpact());
		DefaultEstimate level3b = new DefaultEstimate(
			"Level 3b", "", currency.newCurrencyInstance("100.00"),
			true, new GenericEstimateBehavior(), new ExpenseImpact());
		DefaultEstimate level2 = new DefaultEstimate(
			"Level 2", "", null,
			false, new GenericEstimateBehavior(), new NoImpact());
		level2.addChild(level3a);
		level2.addChild(level3b);
		final DefaultEstimate expected = new DefaultEstimate(
			"Level 1", "", null,
			false, new GenericEstimateBehavior(), new NoImpact());
		expected.addChild(level2);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			DefaultEstimate actual = serializer.read(DefaultEstimate.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			e.printStackTrace(System.err);
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
}
