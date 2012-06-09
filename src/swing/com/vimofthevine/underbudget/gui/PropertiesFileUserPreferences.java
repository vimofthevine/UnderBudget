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

package com.vimofthevine.underbudget.gui;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.Application;

/**
 * User preferences stored in a properties file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class PropertiesFileUserPreferences implements UserPreferences {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(PropertiesFileUserPreferences.class.getName());
	
	/**
	 * User preferences
	 */
	private final Properties properties;
	
	/**
	 * Properties file
	 */
	private final File file;
	
	/**
	 * Constructs a new user preferences
	 * model for the given properties file.
	 * 
	 * @param file location of the properties file
	 */
	public PropertiesFileUserPreferences(String file)
	{
		properties = new Properties();
		this.file = new File(file);
	}

	@Override
    public final void read()
    {
		if (file.exists())
		{
			FileInputStream stream = null;
			
			try
			{
				logger.log(Level.INFO, "Reading user preferences from " +
					file.getAbsolutePath());
				stream = new FileInputStream(file);
				properties.load(stream);
			}
			catch (IOException ioe)
			{
				logger.log(Level.WARNING, "Error reading user preferences file from " +
					file.getAbsolutePath(), ioe);
			}
			finally
			{
				if (stream != null)
				{
					try
					{
						stream.close();
					}
					catch (IOException ioe)
					{
						logger.log(Level.WARNING, "Unable to close file input stream", ioe);
					}
				}
			}
		}
		else
		{
			logger.log(Level.INFO, "No user preferences file found at " +
				file.getAbsolutePath());
		}
    }

	@Override
    public final void save()
    {
		FileOutputStream stream = null;
		try
		{
			stream = new FileOutputStream(file);
			properties.store(stream, Application.TITLE + " user preferences");
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error saving user preferences to " +
				file.getAbsolutePath(), ioe);
		}
		finally
		{
			if (stream != null)
			{
				try
				{
					stream.close();
				}
				catch (IOException ioe)
				{
					logger.log(Level.WARNING, "Unable to close file output stream", ioe);
				}
			}
		}
    }

	@Override
    public final String get(String key, String defaultVal)
    {
		return properties.getProperty(key, defaultVal);
    }

	@Override
    public final void set(String key, String value)
    {
		properties.setProperty(key, value);
    }
	
	@Subscribe
	public final void handleShutdown(ApplicationShutdownEvent event)
	{
		save();
	}

}
