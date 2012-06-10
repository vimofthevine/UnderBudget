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

package com.vimofthevine.underbudget.gui.widgets;

import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import javax.swing.text.SimpleAttributeSet;

/**
 * Document model with simplified API for setting
 * and getting the document text.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SimpleDocument extends PlainDocument {

	/**
	 * Updates the text contained in the document.
	 * 
	 * @param text new document text
	 */
	public void setText(String text)
	{
		try
		{
			super.remove(0, getLength());
			super.insertString(0, text, new SimpleAttributeSet());
		}
		catch (BadLocationException ble)
		{
			// Do nothing
		}
	}
	
	/**
	 * Returns the text contained in the document.
	 * 
	 * @return document text
	 */
	public String getText()
	{
		try
		{
			return getText(0, getLength());
		}
		catch (BadLocationException ble)
		{
			return "";
		}
	}
	
}
