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

package com.vimofthevine.underbudget.core.ledger.transaction;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * A transaction that can be analyzed.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AnalyzableTransaction {
		
	/**
	 * Returns the requested field text in a format that
	 * can be examined by an assignment rule
	 * 
	 * @param field requested transaction field
	 * @return requested field text
	 */
	public String getFieldText(TransactionField field);
	
	/**
	 * Adds the transfer amount of this transaction to
	 * the given currency amount
	 * 
	 * @param total currency amount
	 */
	public void addTransferAmountTo(Currency total);
	
	/**
	 * Compares the date of this transaction to a
	 * given date.
	 * 
	 * @param date a date to be compared against this
	 *              transaction's date
	 * @return a value of negative sign if the date
	 *          of this transaction is earlier than the
	 *          given date, 0 if the date is the same, or
	 *          a value of positive sign if later than
	 *          the given date
	 */
	public int compareTo(SimpleDate date);
	
	/**
	 * Checks if the transaction occurs within the date
	 * constraints of the given budgeting period.
	 * 
	 * @param period date period to be compared
	 * @return true if the transaction occurs within the
	 *          budgeting period, else false
	 */
	public boolean isWithin(BudgetingPeriod period);
	
}
