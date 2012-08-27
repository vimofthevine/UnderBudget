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

package com.vimofthevine.underbudget.swing.summary;

import java.awt.Component;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Factory for building the analysis summary bar
 * displayed on the bottom of the application window.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class AnalysisSummaryViewFactory {
	
	/**
	 * Builds an analysis summary view instance.
	 * 
	 * @param bus      event bus
	 * @param currency currency factory
	 * @param budget   budget
	 * @return analysis summary view component
	 */
	public static final Component build(EventBus bus,
		CurrencyFactory currency, Budget budget)
	{
		AnalysisSummaryViewModel summaryModel =
			new AnalysisSummaryViewModel(bus, budget, currency);
		JPanel summaryComponent = new JPanel();
		new AnalysisSummaryView(summaryComponent, summaryModel);
		return summaryComponent;
	}

}
