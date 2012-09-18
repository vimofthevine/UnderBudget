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

package com.vimofthevine.underbudget.swing.preferences;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class ProgressDisplayPreferences {
	
	/**
	 * Whether to use colors for progress
	 */
	public static final String USE_COLORS = "Progress.Colors";

	/**
	 * Healthy progress color key
	 */
	public static final String HEALTHY_BG_KEY = "Progress.HealthyBG";
	
	/**
	 * Unhealthy progress color key
	 */
	public static final String UNHEALTHY_BG_KEY = "Progress.UnhealthyBG";
	
}
