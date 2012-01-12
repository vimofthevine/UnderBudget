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

package com.vimofthevine.underbudget.util;

import java.awt.Dimension;

import javax.swing.JButton;

/**
 * Convenience utility for creating buttons with given
 * text, action commands, and dimensions
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
abstract public class ButtonFactory {
	
	/**
	 * Creates a new button, given the text, action command,
	 * and size for the button
	 * 
	 * @param text    display text for the button
	 * @param command action command for the button
	 * @param size    preferred dimensions for the button
	 */
	public static JButton createButton(String text, String command, Dimension size)
	{
		JButton button = new JButton(text);
		button.setActionCommand(command);
		button.setPreferredSize(size);
		return button;
	}

}
