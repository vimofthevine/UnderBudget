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
import java.util.Currency;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;

/**
 * Factory for building the detailed analysis results
 * view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class AnalysisResultsViewFactory {
	
	/**
	 * Builds an analysis results view instance.
	 * 
	 * @param bus      event bus
	 * @param currency currency being used
	 * @param budget   budget being analyzed
	 * @return analysis results view component
	 */
	public static final Component build(EventBus bus,
		Currency currency, Budget budget)
	{
		AnalysisResultsViewModel model =
			new AnalysisResultsViewModel(bus, currency, budget);
		JPanel component = new JPanel();
		new AnalysisResultsView(component, model);
		return component;
	}

}
