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

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ByteArrayOutputStream;
import java.io.PrintWriter;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

/**
 * Error popup for displaying detailed error information in an
 * optional section of the popup.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class ErrorPopup {
	
	/**
	 * Displays an error popup for the given
	 * exception.
	 * 
	 * @param exception caught exception to be shown
	 * @param window    application window
	 */
	public static void show(Exception exception, Frame window)
	{
		String message = exception.getMessage();
		String stackTrace = null;
		
		if (exception.getCause() != null)
		{
    		ByteArrayOutputStream stream = new ByteArrayOutputStream();
    		PrintWriter writer = new PrintWriter(stream);
    		exception.printStackTrace(writer);
    		writer.flush();
    		stackTrace = stream.toString();
		}
		
		show(message, stackTrace, window);
	}
	
	/**
	 * Displays an error popup with the given summary and
	 * detailed message.
	 * 
	 * @param message         summary error message
	 * @param detailedMessage detailed error message
	 * @param window          application window
	 */
	public static void show(final String message,
		final String detailedMessage, final Frame window)
	{
		final JDialog dialog = new JDialog(window, "Error", true);
		dialog.setMinimumSize(new Dimension(500, 10));
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		
		JPanel panel = new JPanel(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		if (detailedMessage != null)
		{
       		final JTextArea details = new JTextArea(detailedMessage);
       		details.setEditable(false);
       		final JScrollPane scroll = new JScrollPane(details);
       		scroll.setVisible(false);
       		scroll.setPreferredSize(new Dimension(480, 250));
    		
    		final String expandText = "Details >>>";
    		final String hideText = "<<< Hide";
    		final JButton moreButton = new JButton(expandText);
    		moreButton.addActionListener(new ActionListener() {
    			@Override
    			public void actionPerformed(ActionEvent event)
    			{
    				scroll.setVisible( ! scroll.isVisible());
    				moreButton.setText(scroll.isVisible() ? hideText : expandText);
    				dialog.pack();
    			}
    		});
		
    		c = new GridBagConstraints();
    		c.fill = GridBagConstraints.NONE;
    		c.gridx = 1;
    		c.gridy = 0;
    		c.insets = new Insets(5, 0, 0, 5);
    		panel.add(moreButton, c);
    		
    		c = new GridBagConstraints();
    		c.fill = GridBagConstraints.BOTH;
    		c.gridwidth = 3;
    		c.gridx = 0;
    		c.gridy = 1;
    		c.insets = new Insets(5, 5, 0, 5);
    		c.weightx = 1.0;
    		c.weighty = 1.0;
    		panel.add(scroll, c);
		}
		
		final JButton okButton = new JButton("OK");
		okButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent event)
			{
				dialog.dispose();
			}
		});
		
   		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 0;
		c.insets = new Insets(5, 5, 0, 5);
		c.weightx = 1.0;
		panel.add(new PlainLabel(message), c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridwidth = 3;
		c.gridx = 0;
		c.gridy = 2;
		c.insets = new Insets(5, 5, 5, 5);
		panel.add(okButton, c);
		
		dialog.setContentPane(panel);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.pack();
				dialog.setLocationRelativeTo(window);
				dialog.setVisible(true);
			}
		});
	}

}
