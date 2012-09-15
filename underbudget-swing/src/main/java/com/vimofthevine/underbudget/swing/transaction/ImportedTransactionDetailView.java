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

import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JTextField;
import javax.swing.text.Document;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View for displaying detailed information about
 * a specific transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportedTransactionDetailView {
	
	/**
	 * Constructs a new transaction detail view.
	 * 
	 * @param container Swing component for this view
	 * @param model     transaction detail view model
	 */
	public ImportedTransactionDetailView(Container container,
		ImportedTransactionDetailViewModel model)
	{
		Component date = createField(model.getDateDocument());
		Component payee = createField(model.getPayeeDocument());
		Component memo = createField(model.getMemoDocument());
		Component withdrawal = createField(model.getWithdrawalDocument());
		Component deposit = createField(model.getDepositDocument());
		Component amount = createField(model.getAmountDocument());
		
		GridBagConstraints labels = new GridBagConstraints();
		labels.anchor = GridBagConstraints.LINE_START;
		labels.gridx = 0;
		labels.gridy = GridBagConstraints.RELATIVE;
		labels.insets = new Insets(5, 5, 0, 15);
		
		GridBagConstraints fields = new GridBagConstraints();
		fields.fill = GridBagConstraints.HORIZONTAL;
		fields.gridx = 1;
		fields.gridy = GridBagConstraints.RELATIVE;
		fields.insets = new Insets(5, 0, 0, 5);
		fields.weightx = 1.0;
		
		container.setLayout(new GridBagLayout());
		
		container.add(new BoldLabel("Date"), labels);
		container.add(new BoldLabel("Payee"), labels);
		container.add(new BoldLabel("Memo"), labels);
		container.add(new BoldLabel("Amount"), labels);
		container.add(new BoldLabel("Withdrawal"), labels);
		labels.insets = new Insets(5, 5, 5, 15);
		labels.weighty = 1.0;
		container.add(new BoldLabel("Deposit"), labels);
		
		container.add(date, fields);
		container.add(payee, fields);
		container.add(memo, fields);
		container.add(amount, fields);
		container.add(withdrawal, fields);
		fields.insets = new Insets(5, 0, 5, 5);
		fields.weighty = 1.0;
		container.add(deposit, fields);
	}
	
	/**
	 * Creates an uneditable text field, with
	 * a given document model.
	 * 
	 * @param document text field document model
	 * @return text field
	 */
	private Component createField(Document document)
	{
		JTextField field = new JTextField();
		field.setDocument(document);
		field.setEditable(false);
		return field;
	}

}
