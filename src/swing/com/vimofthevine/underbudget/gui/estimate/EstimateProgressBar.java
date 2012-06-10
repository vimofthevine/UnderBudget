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

import java.awt.Color;

import javax.swing.JProgressBar;

import com.vimofthevine.underbudget.analysis.Progress;
import com.vimofthevine.underbudget.currency.Currency;

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
	public EstimateProgressBar(Progress progress)
	{
		Currency estimated = progress.getEstimatedAmount();
		Currency actual = progress.getActualAmount();
		
		progressBar = new JProgressBar(0, estimated.toInt());
		progressBar.setValue(actual.toInt());
		progressBar.setStringPainted(true);
		progressBar.setString(actual.formatAsString());
		
		Color color = progress.isOk() ? Color.green : Color.red;
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
