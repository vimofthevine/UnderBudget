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

package com.vimofthevine.underbudget;

import java.io.IOException;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Application properties (title, version, etc.)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ApplicationProperties {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(ApplicationProperties.class.getName());
	
	private static final String TITLE = "application.title";
	private static final String VERSION = "application.version";
	private static final String CODENAME = "application.codename";
	private static final String COPYYEAR = "application.copyyear";
	private static final String AUTHOR = "application.author";
	private static final String LICENSE = "application.license";
	private static final String WEBSITE = "application.website";
	
	/**
	 * Application properties
	 */
	private final Properties properties;
	
	/**
	 * Constructs a new application properties instance.
	 */
	public ApplicationProperties()
	{
		properties = new Properties();
		
		try
		{
			properties.load(getClass().getResourceAsStream("application.properties"));
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error reading application properties", ioe);
		}
	}
	
	public final String getTitle()
	{
		return properties.getProperty(TITLE, "Application");
	}
	
	public final String getVersion()
	{
		return properties.getProperty(VERSION, "1.0");
	}
	
	public final String getCodeName()
	{
		return properties.getProperty(CODENAME, "app");
	}

}
