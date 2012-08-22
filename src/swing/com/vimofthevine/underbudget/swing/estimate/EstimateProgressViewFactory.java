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

package com.vimofthevine.underbudget.swing.estimate;

import java.awt.Component;
import java.awt.Frame;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.assignment.EstimateAssignmentRulesView;
import com.vimofthevine.underbudget.swing.assignment.EstimateAssignmentRulesViewModel;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.transaction.AssociatedTransactionsView;
import com.vimofthevine.underbudget.swing.widgets.ComplexSplitPane;

/**
 * Factory for building the estimate progress view,
 * which contains:
 * <ul>
 *   <li>Tree-table of estimates with progress columns</li>
 *   <li>Detail form for modifying a selected estimate</li>
 *   <li>List of assignment rules associated to a selected estimate</li>
 *   <li>List of transactions assigned to a selected estimate</li>
 * </ul>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class EstimateProgressViewFactory {

	/**
	 * Builds an estimate progress view instance.
	 * 
	 * @param window   application window
	 * @param bus      event bus
	 * @param currency currency factory
	 * @param root     root estimate
	 * @param actuals  actual figures source
	 * @param rules    assignment rules list
	 * @return estimate progress view component
	 */
	public static final Component build(Frame window,
		EventBus bus, CurrencyFactory currency, Estimate root,
		ActualFigures actuals, ReverseLookupAssignmentRules rules)
	{
		// Create models
		EstimateProgressTreeTableModel treeTableModel =
			new EstimateProgressTreeTableModel(root, actuals);
		EstimateTreeViewModel treeModel =
			new EstimateTreeViewModel(bus, treeTableModel);
		EstimateDetailViewModel detailModel =
			new EstimateDetailViewModel(bus, currency, window);
		EstimateAssignmentRulesViewModel rulesModel =
			new EstimateAssignmentRulesViewModel(bus, rules);
		AssignedTransactionsViewModel transactionModel =
			new AssignedTransactionsViewModel(bus, rules);
		
		// Create view components
		JPanel treeComponent = new JPanel();
		JPanel detailComponent = new JPanel();
		JPanel rulesComponent = new JPanel();
		JPanel transactionComponent = new JPanel();
		JPanel progressComponent = new JPanel();
		
		// Build views
		new EstimateTreeView(treeComponent, treeModel);
		new EstimateDetailView(detailComponent, detailModel);
		new EstimateAssignmentRulesView(rulesComponent, rulesModel);
		new AssociatedTransactionsView(transactionComponent, transactionModel);
		
		// Put it all together
		ComplexSplitPane split = new ComplexSplitPane(progressComponent);
		split.setTopComponent(treeComponent);
		split.addTab("Estimate", detailComponent);
		split.addTab("Rules", rulesComponent);
		split.addTab("Transactions", transactionComponent);
		
		return progressComponent;
	}
	
}
