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
import java.util.ArrayList;

import org.junit.Before;
import org.junit.Test;
import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.budget.assignment.ComparisonOperator;
import com.vimofthevine.underbudget.core.budget.assignment.UserAssignmentRule;
import com.vimofthevine.underbudget.core.budget.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.CategoryEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.GenericEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.SpecificEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.impact.ExpenseImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.IncomeImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.NoImpact;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;

/**
 * Unit test case for the Simple xml framework
 * serializer used for template budget files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TemplateBudgetSerializerTest {
	
	/**
	 * Template budget XML
	 */
	private final static String XML = BudgetSerializerFactory.PROLOG + "\n" +
		"<budget version=\"4.0\">\n" +
		"   <estimate id=\"0\">\n" +
		"      <name>Root</name>\n" +
		"      <estimates>\n" +
		"         <estimate id=\"1\">\n" +
		"            <name>Root Income</name>\n" +
		"            <estimates>\n" +
		"               <estimate id=\"2\">\n" +
		"                  <name>Income</name>\n" +
		"                  <impact>income</impact>\n" +
		"                  <amount currency=\"USD\">100.00</amount>\n" +
		"                  <behavior type=\"generic\"/>\n" +
		"               </estimate>\n" +
		"            </estimates>\n" +
		"         </estimate>\n" +
		"         <estimate id=\"3\">\n" +
		"            <name>Root Expense</name>\n" +
		"            <estimates>\n" +
		"               <estimate id=\"4\">\n" +
		"                  <name>Expense</name>\n" +
		"                  <impact>expense</impact>\n" +
		"                  <amount currency=\"USD\">80.00</amount>\n" +
		"                  <behavior type=\"specific\"/>\n" +
		"               </estimate>\n" +
		"            </estimates>\n" +
		"         </estimate>\n" +
		"      </estimates>\n" +
		"   </estimate>\n" +
		"   <rules>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>equals</operator>\n" +
		"         <value>text</value>\n" +
		"         <estimate reference=\"4\"/>\n" +
		"      </rule>\n" +
		"   </rules>\n" +
		"</budget>";
	
	/**
	 * Test budget
	 */
	private Budget budget;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public final void setUp()
	{
		MonthlyBudgetingPeriod period = new MonthlyBudgetingPeriod(0, 2011);
		CurrencyFactory factory = new CurrencyFactory("USD");
		Currency initial = factory.newCurrencyInstance("450.00");
		
		DefaultEstimate root = new DefaultEstimate();
		DefaultEstimate rootIncome = new DefaultEstimate("Root Income", "",
			null, false, new CategoryEstimateBehavior(), new NoImpact());
		DefaultEstimate income = new DefaultEstimate("Income", "",
			factory.newCurrencyInstance("100.00"), true, new GenericEstimateBehavior(), new IncomeImpact());
		DefaultEstimate rootExpense = new DefaultEstimate("Root Expense", "",
			null, false, new CategoryEstimateBehavior(), new NoImpact());
		DefaultEstimate expense = new DefaultEstimate("Expense", "",
			factory.newCurrencyInstance("80.00"), false, new SpecificEstimateBehavior(), new ExpenseImpact());
		
		root.addChild(rootIncome);
		rootIncome.addChild(income);
		root.addChild(rootExpense);
		rootExpense.addChild(expense);
		
		ArrayList<AssignmentRule> rules = new ArrayList<AssignmentRule>();
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.EQUALS, "text", expense));
		
		budget = new Budget("Test Budget", period, initial, root, rules);
	}

	/**
	 * Verifies that template budgets are serialized correctly
	 */
	@Test
	public final void testBudgetTemplateSerialization()
	{
		final String expected = XML;
		final Budget input = budget;
		
		Serializer serializer = BudgetSerializerFactory.createTemplateSerializer();
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
	 * Verifies that budget definitions are deserialized correctly
	 */
	@Test
	public final void testBudgetDeserialization()
	{
		final String input = XML;
		CurrencyFactory factory = new CurrencyFactory("USD");
		
		DefaultEstimate root = new DefaultEstimate();
		DefaultEstimate rootIncome = new DefaultEstimate("Root Income", "",
			null, false, new CategoryEstimateBehavior(), new NoImpact());
		DefaultEstimate income = new DefaultEstimate("Income", "",
			factory.newCurrencyInstance("100.00"), false, new GenericEstimateBehavior(), new IncomeImpact());
		DefaultEstimate rootExpense = new DefaultEstimate("Root Expense", "",
			null, false, new CategoryEstimateBehavior(), new NoImpact());
		DefaultEstimate expense = new DefaultEstimate("Expense", "",
			factory.newCurrencyInstance("80.00"), false, new SpecificEstimateBehavior(), new ExpenseImpact());
		
		root.addChild(rootIncome);
		rootIncome.addChild(income);
		root.addChild(rootExpense);
		rootExpense.addChild(expense);
		
		ArrayList<AssignmentRule> rules = new ArrayList<AssignmentRule>();
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.EQUALS, "text", expense));
		
		final Budget expected = new Budget("New Budget", null, null, root, rules);
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		final Budget empty = new Budget("New Budget", new MonthlyBudgetingPeriod(), factory.newCurrencyInstance(), null, null);
		
		try
		{
			// Pass the empty budget with default values
			Budget actual = serializer.read(empty, input);
			
			assertEquals("New Budget", actual.getName());
			assertEquals(new MonthlyBudgetingPeriod(), actual.getBudgetingPeriod());
			assertEquals(factory.newCurrencyInstance(), actual.getInitialBalance());
			assertEquals(expected.getRootEstimate(), actual.getRootEstimate());
			assertEquals(expected.getAssignmentRules(), actual.getAssignmentRules());
		}
		catch (Exception e)
		{
			e.printStackTrace(System.err);
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
}
