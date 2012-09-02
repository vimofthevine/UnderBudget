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

import java.awt.Color;
import java.math.BigDecimal;

import javax.swing.JProgressBar;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;

/**
 * A progress bar that depicts an actual as a percentage
 * of an estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateProgressBar {
	
	/**
	 * Progress bar
	 */
	private final JProgressBar progressBar;
	
	/**
	 * Constructs a new estimate progress bar.
	 * 
	 * @param progress estimate progress
	 */
	public EstimateProgressBar(EstimateProgress progress)
	{
		CashCommodity estimated = progress.getEstimatedAmount();
		CashCommodity actual = progress.getActualAmount();
		
		int max = new BigDecimal(estimated.getValue().asString()).intValue();
		int current = new BigDecimal(actual.getValue().asString()).intValue();
		
		progressBar = new JProgressBar(0, max);
		progressBar.setValue(current);
		progressBar.setStringPainted(true);
		progressBar.setString(actual.formatAsString());
		
		Color color = progress.isHealthy() ? Color.green : Color.red;
		progressBar.setForeground(color);
	}
	
	/**
	 * Returns the progress bar.
	 * 
	 * @return progress bar
	 */
	public JProgressBar getProgressBar()
	{
		return progressBar;
	}
	
}
