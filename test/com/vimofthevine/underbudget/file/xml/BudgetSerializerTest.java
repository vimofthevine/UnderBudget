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
import com.vimofthevine.underbudget.core.budget.assignment.UserAssignmentRule;
import com.vimofthevine.underbudget.core.budget.assignment.ComparisonOperator;
import com.vimofthevine.underbudget.core.budget.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.CategoryEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.EstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.GenericEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.OwedEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.SpecificEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.behavior.UnbudgetedEstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.impact.BalanceImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.ExpenseImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.IncomeImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.NoImpact;
import com.vimofthevine.underbudget.core.budget.estimate.impact.TransferImpact;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the Simple xml framework
 * serializer used for budget files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSerializerTest {
	
	/**
	 * Budget XML
	 */
	private final static String XML = BudgetSerializerFactory.PROLOG + "\n" +
		"<budget version=\"4.0\">\n" +
		"   <name>Test Budget</name>\n" +
		"   <period type=\"monthly\">\n" +
		"      <month>0</month>\n" +
		"      <year>2011</year>\n" +
		"   </period>\n" +
		"   <initial-balance currency=\"USD\">450.00</initial-balance>\n" +
		"   <estimate id=\"0\">\n" +
		"      <name>Root</name>\n" +
		"      <estimates>\n" +
		"         <estimate id=\"1\">\n" +
		"            <name>Income</name>\n" +
		"            <estimates>\n" +
		"               <estimate id=\"2\">\n" +
		"                  <name>Salary</name>\n" +
		"                  <impact>income</impact>\n" +
		"                  <description>From employer</description>\n" +
		"                  <amount currency=\"USD\">400.00</amount>\n" +
		"                  <behavior type=\"generic\"/>\n" +
		"               </estimate>\n" +
		"               <estimate id=\"3\">\n" +
		"                  <name>Unbudgeted Income</name>\n" +
		"                  <impact>income</impact>\n" +
		"                  <amount currency=\"USD\">0.00</amount>\n" +
		"                  <behavior type=\"unbudgeted\"/>\n" +
		"               </estimate>\n" +
		"            </estimates>\n" +
		"         </estimate>\n" +
		"         <estimate id=\"4\">\n" +
		"            <name>Expense</name>\n" +
		"            <estimates>\n" +
		"               <estimate id=\"5\">\n" +
		"                  <name>Groceries</name>\n" +
		"                  <impact>expense</impact>\n" +
		"                  <amount currency=\"USD\">125.00</amount>\n" +
		"                  <final/>\n" +
		"                  <behavior type=\"generic\"/>\n" +
		"               </estimate>\n" +
		"               <estimate id=\"6\">\n" +
		"                  <name>Bills</name>\n" +
		"                  <impact>expense</impact>\n" +
		"                  <amount currency=\"USD\">85.00</amount>\n" +
		"                  <behavior type=\"specific\"/>\n" +
		"               </estimate>\n" +
		"               <estimate id=\"7\">\n" +
		"                  <name>Gifts</name>\n" +
		"                  <impact>expense</impact>\n" +
		"                  <amount currency=\"USD\">20.00</amount>\n" +
		"                  <behavior type=\"owed\">\n" +
		"                     <due>\n" +
		"                        <day>15</day>\n" +
		"                        <month>3</month>\n" +
		"                        <year>2012</year>\n" +
		"                     </due>\n" +
		"                  </behavior>\n" +
		"               </estimate>\n" +
		"               <estimate id=\"8\">\n" +
		"                  <name>Unbudgeted Expense</name>\n" +
		"                  <impact>expense</impact>\n" +
		"                  <amount currency=\"USD\">0.00</amount>\n" +
		"                  <behavior type=\"unbudgeted\"/>\n" +
		"               </estimate>\n" +
		"            </estimates>\n" +
		"         </estimate>\n" +
		"         <estimate id=\"9\">\n" +
		"            <name>Credit Card</name>\n" +
		"            <impact>transfer</impact>\n" +
		"            <amount currency=\"USD\">100.00</amount>\n" +
		"            <behavior type=\"owed\">\n" +
		"               <due>\n" +
		"                  <day>15</day>\n" +
		"                  <month>3</month>\n" +
		"                  <year>2012</year>\n" +
		"               </due>\n" +
		"            </behavior>\n" +
		"         </estimate>\n" +
		"      </estimates>\n" +
		"   </estimate>\n" +
		"   <rules>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>equals</operator>\n" +
		"         <value>Income:Salary</value>\n" +
		"         <estimate reference=\"2\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>begins_with</operator>\n" +
		"         <value>Income</value>\n" +
		"         <estimate reference=\"3\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>ends_with</operator>\n" +
		"         <value>Groceries</value>\n" +
		"         <estimate reference=\"5\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>payee</field>\n" +
		"         <operator>equals</operator>\n" +
		"         <value>Credit Card Payment</value>\n" +
		"         <estimate reference=\"9\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>memo</field>\n" +
		"         <operator>contains</operator>\n" +
		"         <value>bill</value>\n" +
		"         <estimate reference=\"6\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>ends_with</operator>\n" +
		"         <value>Gifts</value>\n" +
		"         <estimate reference=\"7\"/>\n" +
		"      </rule>\n" +
		"      <rule>\n" +
		"         <field>withdrawal</field>\n" +
		"         <operator>begins_with</operator>\n" +
		"         <value>Expense</value>\n" +
		"         <estimate reference=\"8\"/>\n" +
		"      </rule>\n" +
		"   </rules>\n" +
		"</budget>";
	
	/**
	 * Test budget
	 */
	private Budget budget;
	
	/**
	 * Empty budget
	 */
	private Budget emptyBudget;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public final void setUp()
	{
		CurrencyFactory factory = new CurrencyFactory("USD");
		BudgetingPeriod period = new MonthlyBudgetingPeriod(0, 2011);
		Currency initial = factory.newCurrencyInstance("450.00");
		
		EstimateBehavior category = new CategoryEstimateBehavior();
		EstimateBehavior generic = new GenericEstimateBehavior();
		EstimateBehavior owed = new OwedEstimateBehavior(new SimpleDate(15, 3, 2012));
		EstimateBehavior specific = new SpecificEstimateBehavior();
		EstimateBehavior unbudgeted = new UnbudgetedEstimateBehavior();
		
		BalanceImpact incomeImpact = new IncomeImpact();
		BalanceImpact expenseImpact = new ExpenseImpact();
		BalanceImpact transferImpact = new TransferImpact();
		BalanceImpact noImpact = new NoImpact();
		
		DefaultEstimate root = new DefaultEstimate();
		DefaultEstimate income = new DefaultEstimate("Income", "",
			null, false, category, noImpact);
		DefaultEstimate salary = new DefaultEstimate("Salary", "From employer",
			factory.newCurrencyInstance("400.00"), false, generic, incomeImpact);
		DefaultEstimate unbudgetedIncome = new DefaultEstimate("Unbudgeted Income", "",
			factory.newCurrencyInstance("0.00"), false, unbudgeted, incomeImpact);
		DefaultEstimate expense = new DefaultEstimate("Expense", "", null,
			false, category, noImpact);
		DefaultEstimate food = new DefaultEstimate("Groceries", "",
			factory.newCurrencyInstance("125.00"), true, generic, expenseImpact);
		DefaultEstimate bills = new DefaultEstimate("Bills", "",
			factory.newCurrencyInstance("85.00"), false, specific, expenseImpact);
		DefaultEstimate gift = new DefaultEstimate("Gifts", "",
			factory.newCurrencyInstance("20.00"), false, owed, expenseImpact);
		DefaultEstimate unbudgetedExpense = new DefaultEstimate("Unbudgeted Expense", null,
			factory.newCurrencyInstance("0.00"), false, unbudgeted, expenseImpact);
		DefaultEstimate creditPayment = new DefaultEstimate("Credit Card", null,
			factory.newCurrencyInstance("100.00"), false, owed, transferImpact);
		
		// Build estimate tree
		root.addChild(income);
		income.addChild(salary);
		income.addChild(unbudgetedIncome);
		root.addChild(expense);
		expense.addChild(food);
		expense.addChild(bills);
		expense.addChild(gift);
		expense.addChild(unbudgetedExpense);
		root.addChild(creditPayment);
		
		ArrayList<AssignmentRule> rules = new ArrayList<AssignmentRule>();
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.EQUALS, "Income:Salary", salary));
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.BEGINS_WITH, "Income", unbudgetedIncome));
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.ENDS_WITH, "Groceries", food));
		rules.add(new UserAssignmentRule(TransactionField.PAYEE, ComparisonOperator.EQUALS, "Credit Card Payment", creditPayment));
		rules.add(new UserAssignmentRule(TransactionField.MEMO, ComparisonOperator.CONTAINS, "bill", bills));
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.ENDS_WITH, "Gifts", gift));
		rules.add(new UserAssignmentRule(TransactionField.WITHDRAWAL, ComparisonOperator.BEGINS_WITH, "Expense", unbudgetedExpense));
		
		budget = new Budget("Test Budget", period, initial, root, rules);
		emptyBudget = new Budget("New Budget", new MonthlyBudgetingPeriod(), factory.newCurrencyInstance(), null, null);
	}
	
	/**
	 * Verifies that budget definitions are serialized correctly
	 */
	@Test
	public final void testBudgetSerialization()
	{
		final String expected = XML;
		final Budget input = budget;
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		StringWriter writer = new StringWriter();
		
		try
		{
			serializer.write(input, writer);
			assertEquals(expected, writer.toString());
		}
		catch (Exception e)
		{
			e.printStackTrace();
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
		final Budget expected = budget;
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			Budget actual = serializer.read(emptyBudget, input);
			
			assertEquals(expected.getName(), actual.getName());
			assertEquals(expected.getBudgetingPeriod(), actual.getBudgetingPeriod());
			assertEquals(expected.getInitialBalance(), actual.getInitialBalance());
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
