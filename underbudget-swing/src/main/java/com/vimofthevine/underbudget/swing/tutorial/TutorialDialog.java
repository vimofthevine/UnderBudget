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

package com.vimofthevine.underbudget.swing.tutorial;

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import com.vimofthevine.underbudget.swing.window.WindowStateListener;

/**
 * Pop-up dialog for displaying a step-by-step, multi-page
 * tutorial.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class TutorialDialog {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TutorialDialog.class.getName());

	/**
	 * Constructs a new tutorial dialog instance.
	 * 
	 * @param parent parent window
	 * @param model  tutorial view model
	 * @param title  tutorial title
	 */
	TutorialDialog(final Frame parent, TutorialViewModel model,
		String title)
	{
		final JEditorPane editor = new JEditorPane();
		editor.setBackground(UIManager.getColor("Panel.background"));
		editor.setEditable(false);
		
		try
		{
			URL initialUrl = model.getContent().getContent();
			if (initialUrl != null)
			{
    			logger.log(Level.INFO, "Opening " + model.getContent().getContent());
    			editor.setPage(model.getContent().getContent());
			}
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error opening tutorial content", ioe);
		}
		
		model.getContent().addContentListener(new TutorialContentListener() {
			@Override
            public void contentChanged(final URL url)
            {
				try
				{
					editor.setPage(url);
				}
				catch (IOException ioe)
				{
					logger.log(Level.WARNING, "Error opening tutorial content", ioe);
				}
            }
		});
		
		JScrollPane scroll = new JScrollPane(editor);
		scroll.setPreferredSize(new Dimension(600, 500));
		
		JButton back = new JButton(model.getBackAction());
		JButton next = new JButton(model.getNextAction());
		JButton skip = new JButton(model.getSkipAction());
		
		final JDialog dialog = new JDialog(parent, title, true);
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		model.getWindowState().addStateListener(new WindowStateListener() {
			@Override
			public void closeWindow()
			{
				SwingUtilities.invokeLater(new Runnable() {
					public void run()
					{
						dialog.dispose();
					}
				});
			}
		});
		dialog.setLayout(new GridBagLayout());
		
		GridBagConstraints c;
		
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.BOTH;
		c.gridwidth = 3;
		c.weightx = 1.0;
		c.weighty = 1.0;
		dialog.add(scroll, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_START;
		c.gridx = 0;
		c.gridy = 1;
		c.insets = new Insets(5, 5, 5, 0);
		dialog.add(skip, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 1;
		c.gridy = 1;
		c.insets = new Insets(5, 5, 5, 0);
		c.weightx = 1.0;
		dialog.add(back, c);
		
		c = new GridBagConstraints();
		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 2;
		c.gridy = 1;
		c.insets = new Insets(5, 5, 5, 5);
		dialog.add(next, c);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				dialog.pack();
				dialog.setLocationRelativeTo(parent);
				dialog.setVisible(true);
			}
		});
	}

}
