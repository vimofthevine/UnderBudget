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

package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

import com.vimofthevine.underbudget.Application;

/**
 * Writes application version and info to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class VersionWriter {
	
	/**
	 * Writes application version to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println(Application.TITLE + " " + Application.VERSION + " (" + Application.CODENAME + ")");
		stream.println("Copyright (C) " + Application.COPY_YEAR + " " + Application.COPY_AUTHOR);
		stream.println(Application.WEBSITE);
	}

}
