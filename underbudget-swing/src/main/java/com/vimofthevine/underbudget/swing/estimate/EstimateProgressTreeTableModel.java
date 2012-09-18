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

import javax.swing.JProgressBar;

import com.vimofthevine.underbudget.core.assignment.ActualFigure;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;

/**
 * Tree table model for the estimate progress view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateProgressTreeTableModel extends EstimateTreeTableModel {
	
	/**
	 * Currency calculator
	 */
	private final CurrencyCalculator calculator;
	
	/**
	 * Progress bar factory
	 */
	private final ProgressBarFactory progressBars;
	
	/**
	 * Constructs a new estimate progress
	 * tree table model.
	 * 
	 * @param root       root estimate
	 * @param calcualtor currency calculator
	 * @param prefs      user preferences
	 */
	public EstimateProgressTreeTableModel(Estimate root,
		CurrencyCalculator calculator, UserPreferences prefs)
	{
		super(root);
		
		this.calculator = calculator;
		
		progressBars = new ProgressBarFactory(prefs);
	}

	@Override
    public int getColumnCount()
    {
	    return 4;
    }
	
	@Override
	public String getColumnName(int column)
	{
		if (column == 0)
			return "Estimate";
		else if (column == 1)
			return "Progress";
		else if (column == 2)
			return "Estimated Amount";
		else if (column == 3)
			return "Note";
		else
			return "";
	}
	
	@Override
	public Class<?> getColumnClass(int column)
	{
		if (column == 0)
			return String.class;
		else if (column == 1)
			return JProgressBar.class;
		else if (column == 2)
			return CashCommodity.class;
		else if (column == 3)
			return String.class;
		else
			return Object.class;
	}

	@Override
    public Object getValueAt(Object obj, int column)
    {
		if ( ! (obj instanceof Estimate))
			throw new IllegalArgumentException("Only estimates are supported as node objects");
		
		Estimate estimate = (Estimate) obj;
		ActualFigure actual = actuals.getActual(estimate);
		EstimateProgress progress = estimate.getProgress(actual, calculator);
		
		if (column == 0)
		{
			return estimate.getDefinition().getName();
		}
		else if (column == 1)
		{
			return progressBars.get(progress);
		}
		else if (column == 2)
		{
			return progress.getEstimatedAmount();
		}
		else if (column == 3)
		{
			return progress.getNotice();
		}
		else
			throw new IllegalArgumentException("Only 4 columns are supported");
    }
	
}
