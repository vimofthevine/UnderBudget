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
 * Factory for creating progress bar instances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ProgressBarFactory {
	
	/**
	 * Healthy progress bar color
	 */
	private final static Color HEALTHY_COLOR =
		new Color(115, 210, 22);
	
	/**
	 * Unhealthy progress bar color
	 */
	private final static Color UNHEALTHY_COLOR =
		new Color(204, 0, 0);
	
	/**
	 * Progress bar instance
	 */
	private final JProgressBar progressBar;
	
	/**
	 * Constructs a new progress bar factory.
	 */
	public ProgressBarFactory()
	{
		progressBar = new JProgressBar();
		progressBar.setMinimum(0);
		progressBar.setStringPainted(true);
	}
	
	/**
	 * Returns an instance of a progress bar
	 * for the given estimate progress.
	 * 
	 * @param progress estimate progress
	 * @return progress bar instance
	 */
	public JProgressBar get(EstimateProgress progress)
	{
		CashCommodity estimated = progress.getEstimatedAmount();
		CashCommodity actual = progress.getActualAmount();
		
		int max = new BigDecimal(estimated.getValue().asString()).intValue();
		int current = new BigDecimal(actual.getValue().asString()).intValue();
		
		if (max == 0)
		{
			// Can't use 0 as a max
			max++;
		}
		
		progressBar.setMaximum(max);
		progressBar.setValue(current);
		progressBar.setString(actual.formatAsString());
		progressBar.setForeground(progress.isHealthy()
			? HEALTHY_COLOR : UNHEALTHY_COLOR);
		
		return progressBar;
	}

}
