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

package com.vimofthevine.underbudget.swing.analysis;

import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JTextField;
import javax.swing.text.Document;

import com.vimofthevine.underbudget.swing.widgets.BoldLabel;

/**
 * View displaying a summary of the budget and
 * ending balances, displayed at all times along
 * the bottom of the application window (above
 * the status bar).
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AnalysisSummaryView {

	public AnalysisSummaryView(Container container,
		AnalysisSummaryViewModel model)
	{
		container.setLayout(new GridBagLayout());
		GridBagConstraints labelConstraint = new GridBagConstraints();
		labelConstraint.anchor = GridBagConstraints.LINE_START;
		labelConstraint.gridx = GridBagConstraints.RELATIVE;
		labelConstraint.gridy = 0;
		labelConstraint.insets = new Insets(1, 1, 1, 0);
		
		GridBagConstraints fieldConstraint = new GridBagConstraints();
		fieldConstraint.anchor = GridBagConstraints.LINE_START;
		fieldConstraint.fill = GridBagConstraints.HORIZONTAL;
		fieldConstraint.gridx = GridBagConstraints.RELATIVE;
		fieldConstraint.gridy = 0;
		fieldConstraint.insets = new Insets(1, 1, 1, 0);
		fieldConstraint.weightx = 1.0;
		
		container.add(new BoldLabel("Budget:"), labelConstraint);
		container.add(createField(model.getBudgetNameDocument()), fieldConstraint);
		container.add(new BoldLabel("Initial:"), labelConstraint);
		container.add(createField(model.getInitialBalanceDocument()), fieldConstraint);
		container.add(new BoldLabel("Estimated:"), labelConstraint);
		container.add(createField(model.getEstimatedEndingBalanceDocument()), fieldConstraint);
		container.add(new BoldLabel("Actual:"), labelConstraint);
		container.add(createField(model.getActualEndingBalanceDocument()), fieldConstraint);
		container.add(new BoldLabel("Expected:"), labelConstraint);
		container.add(createField(model.getExpectedEndingBalanceDocument()), fieldConstraint);
	}
	
	private Component createField(Document document)
	{
		JTextField field = new JTextField();
		field.setDocument(document);
		field.setEditable(false);
		field.setOpaque(false);
		field.setBorder(BorderFactory.createEmptyBorder());
		return field;
	}
}
