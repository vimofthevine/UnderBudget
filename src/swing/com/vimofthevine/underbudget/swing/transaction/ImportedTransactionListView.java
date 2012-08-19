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
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JScrollPane;

import org.jdesktop.swingx.JXTable;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;
import com.vimofthevine.underbudget.core.util.SimpleDate;
import com.vimofthevine.underbudget.swing.widgets.CurrencyCellRenderer;
import com.vimofthevine.underbudget.swing.widgets.SimpleDateCellRenderer;
import com.vimofthevine.underbudget.swing.widgets.TransferAccountCellRenderer;

/**
 * Table to display the list of imported transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportedTransactionListView {
	
	private final JLabel label;
	
	/**
	 * Constructs a new imported transaction list view.
	 * 
	 * @param container Swing component for the view
	 * @param model     imported transaction list view model
	 */
	public ImportedTransactionListView(Container container,
		ImportedTransactionListViewModel model)
	{
		JXTable table = new JXTable(model.getTableModel());
		table.setSelectionModel(model.getSelectionModel());
		table.setDefaultRenderer(Currency.class, new CurrencyCellRenderer());
		table.setDefaultRenderer(SimpleDate.class, new SimpleDateCellRenderer());
		table.setDefaultRenderer(TransferAccount.class, new TransferAccountCellRenderer());
		
		label = new JLabel();
		label.setFont(label.getFont().deriveFont(Font.PLAIN));
		
		container.setLayout(new BorderLayout());
		container.add(new JScrollPane(table), BorderLayout.CENTER);
	}

}
