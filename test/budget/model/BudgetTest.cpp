/*
 * Copyright 2017 Kyle Treubig
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

// Google include(s)
#include <gtest/gtest.h>

// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include <ledger/model/Money.hpp>
#include <budget/model/Budget.hpp>
#include <budget/model/Expense.hpp>
#include <budget/model/Income.hpp>
#include <budget/model/Recurrence.hpp>

namespace ub {
namespace budget {

/** Test fixture */
class BudgetTest : public ::testing::Test {
protected:
    /** Budget instance */
    Budget budget;

    /**
     * Sets the beginning and ending dates of the budget.
     */
    void SetUp() override {
        budget.reset(QDate(2017, 4, 1), QDate(2017, 5, 31));
    }

    /**
     * Creates an expense instance.
     *
     * @param amount     Amount of the expense
     * @param start      Beginning date of the expense
     * @param stop       Ending date of the expense
     * @param recurrence Recurrence of the expense
     * @return Expense
     */
    Expense expense(double amount, const QDate & start, const QDate & stop,
                    const Recurrence & recurrence) {
        Expense e;
        e.setAmount(ledger::Money(amount));
        e.setBeginningDate(start);
        e.setEndingDate(stop);
        e.setRecurrence(recurrence);
        return e;
    }

    /**
     * Creates an income instance.
     *
     * @param amount     Amount of the income
     * @param start      Beginning date of the income
     * @param stop       Ending date of the income
     * @param recurrence Recurrence of the income
     * @return Expense
     */
    Income income(double amount, const QDate & start, const QDate & stop,
                   const Recurrence & recurrence) {
        Income i;
        i.setAmount(ledger::Money(amount));
        i.setBeginningDate(start);
        i.setEndingDate(stop);
        i.setRecurrence(recurrence);
        return i;
    }
};

/** Verifies that one-time expenses that occur before the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeOneTimeExpensesThatOccurPriorToBudgetBeginningDate) {
    budget.add(expense(100.0, QDate(2017, 3, 1), QDate(2017, 3, 31),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netDecrease().isZero());
}

/** Verifies that recurring expenses that end before the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringExpensesThatEndedPriorToBudgetBeginningDate) {
    budget.add(expense(100.0, QDate(2017, 1, 1), QDate(2017, 3, 31),
                       Recurrence(5, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netDecrease().isZero());
}

/** Verifies that one-tim expenses that occur after the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeOneTimeExpensesThatOccurAfterBudgetEndingDate) {
    budget.add(expense(100.0, QDate(2017, 6, 1), QDate(2017, 6, 30),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netDecrease().isZero());
}

/** Verifies that recurring expenses that start after the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringExpensesThatStartAfterBudgetEndingDate) {
    budget.add(expense(100.0, QDate(2017, 6, 1), QDate(2017, 10, 31),
                       Recurrence(5, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netDecrease().isZero());
}

/** Verifies that recurring expenses that do not occur during the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringExpenseThatDoesNotHaveOccurrenceWithinBudgetDates) {
    budget.add(expense(100.0, QDate(2010, 1, 1), QDate(),
                       Recurrence(19, 9, 1, Recurrence::Yearly, 0)));
    EXPECT_TRUE(budget.netDecrease().isZero());
}

/** Verifies that the net decrease is the sum of all applicable expenses */
TEST_F(BudgetTest, NetDecreaseShouldBeSumOfAllApplicableExpenses) {
    budget.add(expense(10.0, QDate(2017, 4, 1), QDate(2017, 4, 30),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    budget.add(expense(100.0, QDate(2017, 5, 1), QDate(2017, 5, 31),
                       Recurrence(0, 0, 1, Recurrence::Monthly, 0)));
    EXPECT_EQ(ledger::Money(110.0), budget.netDecrease());
}

/** Verifies that recurring expenses that occur right on the budget start date are included */
TEST_F(BudgetTest, ShouldIncludeRecurringExpensesThatOccurOnBudgetBeginningDate) {
    budget.add(expense(1.0, QDate(2017, 4, 1), QDate(),
                       Recurrence(6, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_EQ(ledger::Money(9.0), budget.netDecrease());
}

/** Verifies that recurring expenses with skipping periodicity are included correctly */
TEST_F(BudgetTest, ShouldIncludeRecurringExpensesWithSkippingPeriodicityCorrectly) {
    budget.add(expense(1.0, QDate(2017, 1, 1), QDate(),
                       Recurrence(7, 0, 3, Recurrence::Weekly, 0)));
    EXPECT_EQ(ledger::Money(3.0), budget.netDecrease());
}

/** Verifies that one-time incomes that occur before the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeOneTimeIncomesThatOccurPriorToBudgetBeginningDate) {
    budget.add(income(100.0, QDate(2017, 3, 1), QDate(2017, 3, 31),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netIncrease().isZero());
}

/** Verifies that recurring incomes that end before the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringIncomesThatEndedPriorToBudgetBeginningDate) {
    budget.add(income(100.0, QDate(2017, 1, 1), QDate(2017, 3, 31),
                       Recurrence(5, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netIncrease().isZero());
}

/** Verifies that one-tim incomes that occur after the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeOneTimeIncomesThatOccurAfterBudgetEndingDate) {
    budget.add(income(100.0, QDate(2017, 6, 1), QDate(2017, 6, 30),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netIncrease().isZero());
}

/** Verifies that recurring incomes that start after the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringIncomesThatStartAfterBudgetEndingDate) {
    budget.add(income(100.0, QDate(2017, 6, 1), QDate(2017, 10, 31),
                       Recurrence(5, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_TRUE(budget.netIncrease().isZero());
}

/** Verifies that recurring incomes that do not occur during the budget are not included */
TEST_F(BudgetTest, ShouldNotIncludeRecurringIncomeThatDoesNotHaveOccurrenceWithinBudgetDates) {
    budget.add(income(100.0, QDate(2010, 1, 1), QDate(),
                       Recurrence(19, 9, 1, Recurrence::Yearly, 0)));
    EXPECT_TRUE(budget.netIncrease().isZero());
}

/** Verifies that the net decrease is the sum of all applicable incomes */
TEST_F(BudgetTest, NetIncreaseShouldBeSumOfAllApplicableIncomes) {
    budget.add(income(10.0, QDate(2017, 4, 1), QDate(2017, 4, 30),
                       Recurrence(0, 0, 0, Recurrence::Weekly, 0)));
    budget.add(income(100.0, QDate(2017, 5, 1), QDate(2017, 5, 31),
                       Recurrence(0, 0, 1, Recurrence::Monthly, 0)));
    EXPECT_EQ(ledger::Money(110.0), budget.netIncrease());
}

/** Verifies that recurring incomes that occur right on the budget start date are included */
TEST_F(BudgetTest, ShouldIncludeRecurringIncomesThatOccurOnBudgetBeginningDate) {
    budget.add(income(1.0, QDate(2017, 4, 1), QDate(),
                       Recurrence(6, 0, 1, Recurrence::Weekly, 0)));
    EXPECT_EQ(ledger::Money(9.0), budget.netIncrease());
}

/** Verifies that recurring incomes with skipping periodicity are included correctly */
TEST_F(BudgetTest, ShouldIncludeRecurringIncomesWithSkippingPeriodicityCorrectly) {
    budget.add(income(1.0, QDate(2017, 1, 1), QDate(),
                       Recurrence(7, 0, 3, Recurrence::Weekly, 0)));
    EXPECT_EQ(ledger::Money(3.0), budget.netIncrease());
}

/** Verifies that the net change is the net increase less the net decrease */
TEST_F(BudgetTest, NetChangeShouldBeDifferenceOfNetIncreaseAndNetDecrease) {
    budget.add(income(100.0, QDate(2017, 4, 16), QDate(),
                      Recurrence(0, 0, 0, Recurrence::Yearly, 0)));
    budget.add(expense(34.56, QDate(2017, 5, 27), QDate(),
                       Recurrence(0, 0, 0, Recurrence::Yearly, 0)));
    EXPECT_EQ(ledger::Money(65.44), budget.netChange());
}

} // budget namespace
} // ub namespace
