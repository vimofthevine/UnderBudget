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
		
		GridBagConstraints firstRowLabels = new GridBagConstraints();
		firstRowLabels.gridx = GridBagConstraints.RELATIVE;
		firstRowLabels.gridy = 0;
		
		GridBagConstraints firstRowFields = new GridBagConstraints();
		firstRowFields.fill = GridBagConstraints.HORIZONTAL;
		firstRowFields.gridx = GridBagConstraints.RELATIVE;
		firstRowFields.gridy = 0;
		firstRowFields.weightx = 1.0;
		
		GridBagConstraints secondRowLabels = new GridBagConstraints();
		secondRowLabels.gridx = GridBagConstraints.RELATIVE;
		secondRowLabels.gridy = 1;
		
		GridBagConstraints secondRowFields = new GridBagConstraints();
		secondRowFields.fill = GridBagConstraints.HORIZONTAL;
		secondRowFields.gridx = GridBagConstraints.RELATIVE;
		secondRowFields.gridy = 1;
		secondRowFields.weightx = 1.0;
		
		container.setLayout(new GridBagLayout());
		
		container.add(new BoldLabel("Date"), firstRowLabels);
		container.add(date, firstRowFields);
		container.add(new BoldLabel("Payee"), firstRowLabels);
		container.add(payee, firstRowFields);
		container.add(new BoldLabel("Withdrawal"), firstRowLabels);
		container.add(withdrawal, firstRowFields);
		
		container.add(new BoldLabel("Amount"), secondRowLabels);
		container.add(amount, secondRowFields);
		container.add(new BoldLabel("Memo"), secondRowLabels);
		container.add(memo, secondRowFields);
		container.add(new BoldLabel("Deposit"), secondRowLabels);
		container.add(deposit, secondRowFields);
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
