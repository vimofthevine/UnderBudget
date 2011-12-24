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
