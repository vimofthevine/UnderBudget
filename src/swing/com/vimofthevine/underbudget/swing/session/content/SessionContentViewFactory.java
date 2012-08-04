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

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.actuals.ActualFigures;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.estimate.BalanceImpactViewFactory;
import com.vimofthevine.underbudget.swing.estimate.EstimateProgressViewFactory;
import com.vimofthevine.underbudget.swing.status.StatusBar;
import com.vimofthevine.underbudget.swing.status.StatusBarModel;

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
		EventBus bus, CurrencyFactory currency, Budget budget,
		ActualFigures actuals)
	{
		SessionContentViewModel model = new SessionContentViewModel(bus);
		
		Component estimateProgress = EstimateProgressViewFactory.build(
			window, bus, currency, budget.getRootEstimate(), actuals);
		Component balanceImpact = BalanceImpactViewFactory.build(
			window, bus, currency, budget.getRootEstimate(), actuals);
		
		JPanel content = new JPanel();
		SessionContentView view = new SessionContentView(content, model);
		
		view.add(new JLabel("Analysis Summary"), SessionContent.ANALYSIS_SUMMARY);
		view.add(new JLabel("Assignment Rules"), SessionContent.ASSIGNMENT_RULES);
		view.add(balanceImpact, SessionContent.BALANCE_IMPACT);
		view.add(estimateProgress, SessionContent.ESTIMATE_PROGRESS);
		view.add(new JLabel("Imported Transactions"), SessionContent.IMPORTED_TRANSACTIONS);
		
		JPanel statusBar = new JPanel();
		statusBar.setBorder(BorderFactory.createEmptyBorder(0, 2, 2, 2));
		StatusBar status = new StatusBar(statusBar);
		StatusBarModel statusModel = new StatusBarModel(status);
		
		JPanel composite = new JPanel(new BorderLayout());
		composite.add(content, BorderLayout.CENTER);
		composite.add(statusBar, BorderLayout.SOUTH);
		
		return composite;
	}

}
