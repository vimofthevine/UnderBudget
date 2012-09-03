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

package com.vimofthevine.underbudget.swing.session.content;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Frame;
import java.util.Currency;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.swing.UserPreferences;
import com.vimofthevine.underbudget.swing.assignment.AssignmentRulesViewFactory;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.estimate.BalanceImpactViewFactory;
import com.vimofthevine.underbudget.swing.estimate.EstimateProgressViewFactory;
import com.vimofthevine.underbudget.swing.status.StatusBar;
import com.vimofthevine.underbudget.swing.status.StatusBarModel;
import com.vimofthevine.underbudget.swing.summary.AnalysisResultsViewFactory;
import com.vimofthevine.underbudget.swing.summary.AnalysisSummaryViewFactory;
import com.vimofthevine.underbudget.swing.transaction.ImportedTransactionsViewFactory;

/**
 * Factory for building an entire session content view,
 * which contains:
 * <ul>
 *   <li>Estimate progress view</li>
 *   <li>Balance impact view</li>
 *   <li>Assignment rules view</li>
 *   <li>Imported transactions view</li>
 *   <li>Analysis summary view</li>
 * </ul>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class SessionContentViewFactory {
	
	public static final Component build(Frame window,
		EventBus bus, Currency currency, Budget budget,
		ReverseLookupAssignmentRules rules,
		CurrencyCalculator calculator, UserPreferences prefs)
	{
		SessionContentViewModel model = new SessionContentViewModel(bus);
		
		Component analysisSummary = AnalysisSummaryViewFactory.build(bus,
			currency, budget);
		Component estimateProgress = EstimateProgressViewFactory.build(
			window, bus, currency, budget.getRootEstimate(), rules,
			calculator, prefs);
		Component balanceImpact = BalanceImpactViewFactory.build(
			window, bus, currency, budget.getRootEstimate(), rules, calculator);
		Component assignmentRules = AssignmentRulesViewFactory.build(
			window, bus, rules, currency);
		Component importedTransactions = ImportedTransactionsViewFactory.build(
			window, bus, rules, currency);
		Component analysisResults = AnalysisResultsViewFactory.build(
			bus, currency, budget);
		
		JPanel swapableContent = new JPanel();
		SessionContentView view = new SessionContentView(swapableContent, model);
		
		view.add(analysisResults, SessionContent.ANALYSIS_SUMMARY);
		view.add(assignmentRules, SessionContent.ASSIGNMENT_RULES);
		view.add(balanceImpact, SessionContent.BALANCE_IMPACT);
		view.add(estimateProgress, SessionContent.ESTIMATE_PROGRESS);
		view.add(importedTransactions, SessionContent.IMPORTED_TRANSACTIONS);
		
		JPanel statusBar = new JPanel();
		statusBar.setBorder(BorderFactory.createEmptyBorder(0, 2, 2, 2));
		StatusBar status = new StatusBar(statusBar);
		StatusBarModel statusModel = new StatusBarModel(status);
		
		JPanel sessionContent = new JPanel(new BorderLayout());
		sessionContent.add(swapableContent, BorderLayout.CENTER);
		sessionContent.add(analysisSummary, BorderLayout.SOUTH);
		
		JPanel composite = new JPanel(new BorderLayout());
		composite.add(sessionContent, BorderLayout.CENTER);
		composite.add(statusBar, BorderLayout.SOUTH);
		
		return composite;
	}

}
