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

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.text.Document;

/**
 * Model for a text input field.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TextInputModel implements InputFieldListener {
	
	/**
	 * Document for the text field
	 */
	private final SimpleDocument document;
	
	/**
	 * Action model for the text field
	 */
	private final Action action;
	
	/**
	 * Constructs a new text input model.
	 */
	public TextInputModel()
	{
		document = new SimpleDocument();
		document.addInputFieldListener(this);
		
		action = new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				// Do nothing
			}
		};
	}
	
	@Override
	public void fieldChanged()
	{
		// Do nothing
	}
	
	/**
	 * Updates the text shown in the input field.
	 * 
	 * @param text new field text
	 */
	public final void setText(String text)
	{
		document.setText(text);
	}
	
	/**
	 * Returns the text in the input field.
	 * 
	 * @return input text
	 */
	public final String getText()
	{
		return document.getText();
	}
	
	/**
	 * Specifies whether the input field is
	 * enabled or disabled.
	 * 
	 * @param enabled <code>true</code> if the
	 *                field is enabled
	 */
	public final void setEnabled(boolean enabled)
	{
		action.setEnabled(enabled);
	}
	
	/**
	 * Returns the text document model.
	 * 
	 * @return text document model
	 */
	final Document getDocument()
	{
		return document;
	}
	
	/**
	 * Returns the input action model.
	 * 
	 * @return input action model
	 */
	final Action getAction()
	{
		return action;
	}

}
