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

import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Presentation model for views that display detailed
 * analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AnalysisResultsViewModel {
	
	/**
	 * Ending balances table model
	 */
	private final EndingBalancesTableModel balancesTableModel;
	
	/**
	 * Constructs a new analysis results view model.
	 * 
	 * @param bus     event bus
	 * @param factory currency factory
	 */
	public AnalysisResultsViewModel(EventBus bus,
		CurrencyFactory factory)
	{
		bus.register(this);
		
		balancesTableModel = new EndingBalancesTableModel(factory);
	}
	
	/**
	 * Updates the ending balances table when the balances
	 * have been re-calculated.
	 * 
	 * @param event balances calculated event
	 */
	@Subscribe
	public void balancesCalculated(BalancesCalculatedEvent event)
	{
		balancesTableModel.setResults(event.getEndingBalances());
	}
	
	/**
	 * Returns a table model representing the
	 * ending balances.
	 * 
	 * @return ending balances table model
	 */
	public TableModel getEndingBalancesTableModel()
	{
		return balancesTableModel;
	}
	
}
