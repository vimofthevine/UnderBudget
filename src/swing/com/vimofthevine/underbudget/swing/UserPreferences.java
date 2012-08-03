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

package com.vimofthevine.underbudget.swing;

/**
 * User preferences chosen by the user and preserved
 * through application executions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface UserPreferences {

	/**
	 * Reads all user preferences from
	 * their source.
	 */
	public void read();
	
	/**
	 * Saves all user preferences so they
	 * will be retained.
	 */
	public void save();
	
	/**
	 * Retrieves a user preference, given the
	 * property key and a default in the event
	 * that no preference property has been set.
	 * 
	 * @param key        user preference property key
	 * @param defaultVal default value if none set
	 */
	public String get(String key, String defaultVal);
	
	/**
	 * Updates a user preference to the given
	 * value.
	 * 
	 * @param key   user preference property key
	 * @param value new user preference value
	 */
	public void set(String key, String value);
	
}
