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

package com.vimofthevine.underbudget.swing.assignment;

import java.awt.Component;
import java.awt.Frame;

import javax.swing.JLabel;
import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.estimate.EstimateDetailView;
import com.vimofthevine.underbudget.swing.estimate.EstimateDetailViewModel;
import com.vimofthevine.underbudget.swing.widgets.ComplexSplitPane;

/**
 * Factory for building the assignment rules view,
 * which contains:
 * <ul>
 *   <li>List of assignment rules</li>
 *   <li>Detail form for modifying a selected rule</li>
 *   <li>Detail form for modifying the associated estimate of a selected rule</li>
 *   <li>List of transactions assigned by the selected rule</li>
 * </ul>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class AssignmentRulesViewFactory {
	
	/**
	 * Builds an assignment rules view instance.
	 * 
	 * @param window   application window
	 * @param bus      event bus
	 * @param rules    assignment rules
	 * @param currency currency factory
	 * @return assignment rules view component
	 */
	public static final Component build(Frame window,
		EventBus bus, AssignmentRules rules, CurrencyFactory currency)
	{
		// Create models
		AssignmentRuleListViewModel listModel =
			new AssignmentRuleListViewModel(bus, rules);
		AssignmentRuleDetailViewModel detailModel =
			new AssignmentRuleDetailViewModel(bus, window, rules);
		EstimateDetailViewModel estimateModel =
			new EstimateDetailViewModel(bus, currency, window);
		
		// Create view components
		JPanel listComponent = new JPanel();
		JPanel detailComponent = new JPanel();
		JPanel estimateComponent = new JPanel();
		JPanel rulesComponent = new JPanel();
		
		// Build views
		new AssignmentRuleListView(listComponent, listModel);
		new AssignmentRuleDetailView(detailComponent, detailModel);
		new EstimateDetailView(estimateComponent, estimateModel);
		
		// Put it all together
		ComplexSplitPane split = new ComplexSplitPane(rulesComponent);
		split.setTopComponent(listComponent);
		split.addTab("Rule", detailComponent);
		split.addTab("Estimate", estimateComponent);
		split.addTab("Transactions", new JLabel("Associated transactions"));
		
		return rulesComponent;
	}

}
