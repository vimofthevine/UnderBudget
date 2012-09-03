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

package com.vimofthevine.underbudget.stubs.transaction.source;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.date.DateRange;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.source.TransactionSource;
import com.vimofthevine.underbudget.stubs.transaction.StubTransaction;
import com.vimofthevine.underbudget.stubs.util.StubSimpleDate;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubTransactionSource implements TransactionSource {

	@Override
	public Transaction[] getTransactions(DateRange range)
	{
		return new Transaction[] {
			// Income
			new StubTransaction(new StubSimpleDate(2012, 8, 10),
				"Payday", "", "Income:Salary",
				"Assets:Current Assets:TD Checking", cash("1002.60")),
				
			// Expense
			new StubTransaction(new StubSimpleDate(2012, 8, 14),
				"Costco", "", "Assets:Current Assets:TD Checking",
				"Expenses:Groceries", cash("84.56")),
				
			// Expense
			new StubTransaction(new StubSimpleDate(2012, 8, 14),
				"Christmas Tree Shops", "", "Liabilities:Capital One Venture",
				"Expenses:Housewares", cash("24.39")),
				
			// Refund
			new StubTransaction(new StubSimpleDate(2012, 8, 16),
				"Banana Republic", "", "Liabilities:Banana Republic",
				"Expenses:Clothes", cash("32.00")),
				
			// Payment
			new StubTransaction(new StubSimpleDate(2012, 8, 17),
				"Capital One Payment", "", "Assets:Current Assets:TD Checking",
				"Liabilities:Capital One Venture", cash("398.86")),
				
		};
	}
	
	private CashCommodity cash(String amount)
	{
		return Commodity.create("USD", amount);
	}

}
