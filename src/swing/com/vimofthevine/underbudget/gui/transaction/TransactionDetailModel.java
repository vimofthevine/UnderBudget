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

package com.vimofthevine.underbudget.gui.transaction;

import javax.swing.text.Document;

/**
 * Presentation model for views that display
 * details about an individual transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionDetailModel {

	/**
	 * Returns a document representing
	 * the transaction's date posted.
	 * 
	 * @return transaction date document
	 */
	public Document getDateDocument();
	
	/**
	 * Returns a document representing
	 * the transaction's payee.
	 * 
	 * @return transaction payee document
	 */
	public Document getPayeeDocument();
	
	/**
	 * Returns a document representing
	 * the transaction's memo.
	 * 
	 * @return transaction memo document
	 */
	public Document getMemoDocument();
	
	/**
	 * Returns a document representing
	 * the transaction's amount.
	 * 
	 * @return transaction amount document
	 */
	public Document getAmountDocument();
	
	/**
	 * Returns a document representing
	 * the transaction's withdrawal account.
	 * 
	 * @return transaction withdrawal document
	 */
	public Document getWithdrawalDocument();
	
	/**
	 * Returns a document representing
	 * the transaction's deposit account.
	 * 
	 * @return transaction deposit document
	 */
	public Document getDepositDocument();
	
}
