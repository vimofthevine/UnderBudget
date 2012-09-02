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
import java.awt.Frame;
import java.util.Currency;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.swing.assignment.AssignmentRuleDetailView;
import com.vimofthevine.underbudget.swing.assignment.AssignmentRuleDetailViewModel;
import com.vimofthevine.underbudget.swing.estimate.EstimateDetailView;
import com.vimofthevine.underbudget.swing.estimate.EstimateDetailViewModel;
import com.vimofthevine.underbudget.swing.widgets.ComplexSplitPane;

/**
 * Factory for building the imported transactions view,
 * which contains:
 * <ul>
 *   <li>List of imported transactions</li>
 *   <li>Detail view of a selected transaction</li>
 *   <li>Detail view of the assigning rule of a selected transaction</li>
 *   <li>Detail view of the associated estimate of a selected transaction</li>
 * </ul>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class ImportedTransactionsViewFactory {
	
	/**
	 * Builds an imported transactions view instance.
	 * 
	 * @param window   application window
	 * @param bus      event bus
	 * @param rules    assignment rule list
	 * @param currency currency in use
	 * @return imported transactions view component
	 */
	public static final Component build(Frame window,
		EventBus bus, AssignmentRules rules, Currency currency)
	{
		// Create models
		ImportedTransactionListViewModel listModel =
			new ImportedTransactionListViewModel(bus);
		ImportedTransactionDetailViewModel detailModel =
			new ImportedTransactionDetailViewModel(bus);
		AssignmentRuleDetailViewModel ruleModel =
			new AssignmentRuleDetailViewModel(bus, window, rules);
		EstimateDetailViewModel estimateModel =
			new EstimateDetailViewModel(bus, currency, window);
		
		// Create view components
		JPanel listComponent = new JPanel();
		JPanel detailComponent = new JPanel();
		JPanel ruleComponent = new JPanel();
		JPanel estimateComponent = new JPanel();
		JPanel transactionsComponent = new JPanel();
		
		// Build views
		new ImportedTransactionListView(listComponent, listModel);
		new ImportedTransactionDetailView(detailComponent, detailModel);
		new AssignmentRuleDetailView(ruleComponent, ruleModel);
		new EstimateDetailView(estimateComponent, estimateModel);
		
		// Put it all together
		ComplexSplitPane split = new ComplexSplitPane(transactionsComponent);
		split.setTopComponent(listComponent);
		split.addTab("Transaction", detailComponent);
		split.addTab("Rule", ruleComponent);
		split.addTab("Estimate", estimateComponent);
		
		return transactionsComponent;
	}

}
