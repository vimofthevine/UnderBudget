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

package com.vimofthevine.underbudget.gui.estimate;

import com.vimofthevine.underbudget.analysis.ActualFigure;
import com.vimofthevine.underbudget.analysis.ActualFigureSource;
import com.vimofthevine.underbudget.currency.Currency;
import com.vimofthevine.underbudget.estimate.Estimate;

/**
 * Tree table model for the estimate progress view.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateProgressTreeTableModel extends AbstractEstimateTreeTableModel {
	
	/**
	 * Actual figures source
	 */
	private final ActualFigureSource actuals;
	
	/**
	 * Constructs a new estimate progress
	 * tree table model.
	 * 
	 * @param root    root estimate
	 * @param actuals actual figures source
	 */
	public EstimateProgressTreeTableModel(Estimate root, ActualFigureSource actuals)
	{
		super(root);
		this.actuals = actuals;
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
			return EstimateProgressBar.class;
		else if (column == 2)
			return Currency.class;
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
		
		if (column == 0)
		{
			return estimate.getName();
		}
		else if (column == 1)
		{
			ActualFigure actual = actuals.getActualFigure(estimate);
			return new EstimateProgressBar(estimate.getProgress(actual));
		}
		else if (column == 2)
		{
			return estimate.getAmount();
		}
		else if (column == 3)
		{
			ActualFigure actual = actuals.getActualFigure(estimate);
			return estimate.getProgress(actual).getNotice();
		}
		else
			throw new IllegalArgumentException("Only 4 columns are supported");
    }
	
}
