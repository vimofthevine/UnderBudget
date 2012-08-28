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

package com.vimofthevine.underbudget.swing.transaction;

import java.awt.BorderLayout;
import java.awt.Container;

import javax.swing.JScrollPane;

import org.jdesktop.swingx.JXTable;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;
import com.vimofthevine.underbudget.swing.widgets.CurrencyCellRenderer;
import com.vimofthevine.underbudget.swing.widgets.SimpleDateCellRenderer;
import com.vimofthevine.underbudget.swing.widgets.TransferAccountCellRenderer;

/**
 * View for displaying transactions associated
 * with a specific item.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssociatedTransactionsView {
	
	/**
	 * Constructs a new associated transactions view.
	 * 
	 * @param container Swing component for the view
	 * @param model     associated transactions view model
	 */
	public AssociatedTransactionsView(Container container,
		AssociatedTransactionsViewModel model)
	{
		JXTable table = new JXTable(model.getTransactionsTableModel());
		table.setSelectionModel(model.getTransactionSelectionModel());
		table.setDefaultRenderer(Currency.class, new CurrencyCellRenderer());
		table.setDefaultRenderer(SimpleDate.class, new SimpleDateCellRenderer());
		table.setDefaultRenderer(TransferAccount.class, new TransferAccountCellRenderer());
		
		container.setLayout(new BorderLayout());
		container.add(new JScrollPane(table), BorderLayout.CENTER);
	}

}
