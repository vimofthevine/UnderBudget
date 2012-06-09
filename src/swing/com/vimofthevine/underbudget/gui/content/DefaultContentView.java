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

package com.vimofthevine.underbudget.gui.content;

import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;

import javax.swing.SwingUtilities;

/**
 * This feels a little hokey. Come back and revisit it.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultContentView implements ContentView {
	
	/**
	 * Content view
	 */
	private final Container view;
	
	/**
	 * Card layout
	 */
	private final CardLayout layout;
	
	/**
	 * Constructs a new content view.
	 */
	public DefaultContentView(Container contentView)
	{
		view = contentView;
		layout = new CardLayout();
		view.setLayout(layout);
	}

	@Override
    public void addDisplay(final Component display, final ContentDisplay name)
    {
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				view.add(display, name.toString());
				//layout.addLayoutComponent(display, name.toString());
			}
		});
    }

	@Override
    public void showDisplay(final ContentDisplay name)
    {
		layout.show(view, name.toString());
    }

}
