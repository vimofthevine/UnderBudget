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
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JProgressBar;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;
import com.vimofthevine.underbudget.swing.UserPreferences;
import com.vimofthevine.underbudget.swing.preferences.ProgressDisplayPreferences;

/**
 * Factory for creating progress bar instances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ProgressBarFactory {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(ProgressBarFactory.class.getName());
	
	/**
	 * Default healthy progress bar color
	 */
	private final static Color HEALTHY_COLOR =
		new Color(115, 210, 22);
	
	/**
	 * Default unhealthy progress bar color
	 */
	private final static Color UNHEALTHY_COLOR =
		new Color(204, 0, 0);
	
	/**
	 * Progress bar instance
	 */
	private final JProgressBar progressBar;
	
	/**
	 * Whether to use colors
	 */
	private boolean useColors;
	
	/**
	 * Color to be used for healthy progress
	 */
	private Color healthyColor;
	
	/**
	 * Color to be used for unhealthy progress
	 */
	private Color unhealthyColor;
	
	/**
	 * Constructs a new progress bar factory.
	 * 
	 * @param prefs user preferences
	 */
	ProgressBarFactory(UserPreferences prefs)
	{
		progressBar = new JProgressBar();
		progressBar.setMinimum(0);
		progressBar.setStringPainted(true);
		progressBar.setUI(new EstimateProgressBarUI());
		
		getColors(prefs);
	}
	
	/**
	 * Retrieves the progress bar parameters from
	 * the user preferences.
	 * 
	 * @param prefs user preferences
	 */
	private void getColors(UserPreferences prefs)
	{
		String useColorsStr = prefs.get(
			ProgressDisplayPreferences.USE_COLORS, "true");
		String healthyColorStr = prefs.get(
			ProgressDisplayPreferences.HEALTHY_BG_KEY, getString(HEALTHY_COLOR));
		String unhealthyColorStr = prefs.get(
			ProgressDisplayPreferences.UNHEALTHY_BG_KEY, getString(UNHEALTHY_COLOR));
		
		logger.log(Level.FINEST, "Use colors for progress? " + useColorsStr);
		logger.log(Level.FINEST, "Healthy progress color: " + healthyColorStr);
		logger.log(Level.FINEST, "Unhealthy progress color: " + unhealthyColorStr);
		
		useColors = Boolean.parseBoolean(useColorsStr);
		healthyColor = getColor(healthyColorStr);
		unhealthyColor = getColor(unhealthyColorStr);
	}
	
	/**
	 * Converts a given color into an RGB string.
	 * 
	 * @param color color to be represented as an RGB string
	 * @return RGB string representation of the given color
	 */
	private String getString(Color color)
	{
		return color.getRed() + ","
			+ color.getGreen() + ","
			+ color.getBlue();
	}
	
	/**
	 * Converts a given RBG string into a color.
	 * 
	 * @param rgb RBG string to be converted to a color
	 * @return color represented by the RBG string
	 */
	private Color getColor(String rgb)
	{
		String[] comps = rgb.split(",");
		
		if (comps.length != 3)
			throw new IllegalArgumentException("Invalid color string given, " + rgb);
		
		int red = Integer.parseInt(comps[0]);
		int green = Integer.parseInt(comps[1]);
		int blue = Integer.parseInt(comps[2]);
		
		return new Color(red, green, blue);
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
		
		if (useColors)
		{
    		progressBar.setForeground(progress.isHealthy()
    			? healthyColor : unhealthyColor);
		}
		
		return progressBar;
	}

}
