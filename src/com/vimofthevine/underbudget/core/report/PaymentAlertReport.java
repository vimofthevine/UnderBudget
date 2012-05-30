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

package com.vimofthevine.underbudget.core.report;

import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * A payment alert report records upcoming and
 * overdue expenses.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface PaymentAlertReport {
	
	/**
	 * Records an upcoming expense that is due on the given date.
	 * 
	 * @param name name of the expense
	 * @param date due date of the expense
	 */
	public void recordUpcomingExpense(String name, SimpleDate date);
	
	/**
	 * Records an overdue expense that was due on the given date.
	 * 
	 * @param name name of the expense
	 * @param date due date of the expense
	 */
	public void recordOverdueExpense(String name, SimpleDate date);

}
