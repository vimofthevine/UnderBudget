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

package com.vimofthevine.underbudget.swing.widgets;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

import javax.swing.text.AttributeSet;
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
	 * Registered field change listeners
	 */
	private final Collection<InputFieldListener> listeners;
	
	/**
	 * Constructs a new simple document model.
	 */
	public SimpleDocument()
	{
		super();
		
		listeners = Collections.synchronizedList(new ArrayList<InputFieldListener>());
	}
	
	/**
	 * Registers a listener to be notified when the
	 * input field is modified by the user.
	 * 
	 * @param listener input field listener
	 */
	public void addInputFieldListener(InputFieldListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from being notified when the
	 * input field is modified by the user.
	 * 
	 * @param listener input field listener
	 */
	public void removeInputFieldListener(InputFieldListener listener)
	{
		listeners.remove(listener);
	}
	
	/**
	 * Notifies all registered input field listeners
	 * that the field has been updated.
	 */
	private void update()
	{
		synchronized (listeners)
		{
			for (InputFieldListener listener : listeners)
			{
				listener.fieldChanged();
			}
		}
	}
	
	@Override
	public void insertString(int offset, String string,
		AttributeSet attributes) throws BadLocationException
	{
		super.insertString(offset, string, attributes);
		update();
	}
	
	@Override
	public void remove(int offset, int length)
	throws BadLocationException
	{
		super.remove(offset, length);
		update();
	}

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
