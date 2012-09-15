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

package com.vimofthevine.underbudget.core.transaction.source.csv;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * List of user-defined CSV profiles. User-defined
 * CSV profiles are stored in the application settings
 * directory under "csvprofiles".
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UserProfileList {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(UserProfileList.class.getName());
	
	/**
	 * User CSV profiles directory
	 */
	private final File dir;
	
	/**
	 * Constructs a user profile list instance.
	 * 
	 * @param settingsDir application settings directory
	 */
	public UserProfileList(String settingsDir)
	{
		dir = new File(settingsDir + "/csvprofiles");
		
		if ( ! dir.exists())
		{
			dir.mkdir();
		}
	}
	
	/**
	 * Reads from the application settings directory
	 * for user CSV profiles.
	 * 
	 * @return user CSV profiles
	 */
	public UserCsvProfile[] getList()
	{
		String[] files = dir.list(new FilenameFilter() {
			@Override
            public boolean accept(File directory, String name)
            {
				return name.endsWith(".csvprofile");
            }
		});
		
		UserCsvProfile[] profiles
			= new UserCsvProfile[files.length];
		
		for (int i=0; i<profiles.length; i++)
		{
			String file = dir.getAbsolutePath() + "/" + files[i];
			
			try
			{
				profiles[i] = new UserCsvProfile(file);
			}
			catch (IOException ioe)
			{
				logger.log(Level.WARNING, "Unable to open CSV profile, " + file, ioe);
			}
		}
		
		 return profiles;
	}
	
}
