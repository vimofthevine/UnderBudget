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

import java.awt.Component;
import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.SwingUtilities;

/**
 * Convenience utility for displaying a popup dialog
 * prompting the user for a password.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class PasswordPrompt {
	
	/**
	 * Displays a popup dialog prompting the user for
	 * a password. This method must be run on the event
	 * dispatch thread.
	 * 
	 * @param prompt prompt text to be displayed
	 * @param parent parent component
	 * @return given password, or null if the user hits "cancel"
	 */
	public static String prompt(String prompt, Component parent)
	{
		JPanel panel = new JPanel();
		JLabel label = new JLabel(prompt);
		final JPasswordField password = new JPasswordField(10);
		panel.add(label);
		panel.add(password);
		
		password.addHierarchyListener(new HierarchyListener() {
			@Override
            public void hierarchyChanged(final HierarchyEvent event)
            {
				final HierarchyListener listener = this;
				if ((HierarchyEvent.SHOWING_CHANGED & event.getChangeFlags()) != 0
					&& password.isShowing())
				{
					Timer timer = new Timer();
					timer.schedule(new TimerTask() {
						public void run()
						{
							SwingUtilities.invokeLater(new Runnable() {
								public void run()
								{
                					password.requestFocusInWindow();
                					password.removeHierarchyListener(listener);
								}
							});
						}
					}, 100);
				}
            }
		});
		
		int result = JOptionPane.showOptionDialog(parent, panel, prompt,
			JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE,
			null, null, "");
		
		if (result == JOptionPane.OK_OPTION)
		{
			char[] passkey = password.getPassword();
			return new String(passkey);
		}
		else
			return null;
	}

}
