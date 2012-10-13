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

package com.vimofthevine.underbudget.swing.session.source.aes;

import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;


/**
 * File chooser for selecting AES encrypted budget files.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AesEncryptedFileChooser extends JFileChooser {

	/**
	 * Constructs a new AES encrypted budget file chooser.
	 * 
	 * @param directory starting directory
	 */
	AesEncryptedFileChooser(String directory)
	{
		// Add the accept-all file filter
		setAcceptAllFileFilterUsed(true);
		
		// Set up encrypted file filter
		FileFilter fileFilter = new AesEncryptedFileFilter();
		addChoosableFileFilter(fileFilter);
		setFileFilter(fileFilter);
		
		// Set directory
		File dir = new File(directory);
		if (dir.exists())
		{
			setCurrentDirectory(dir);
		}
	}

}