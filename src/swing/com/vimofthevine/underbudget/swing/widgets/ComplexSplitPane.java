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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;

import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;

/**
 * Complex split pane, with a top portion, and a
 * bottom portion. The bottom portion is a tabbed
 * pane.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ComplexSplitPane {
	
	/**
	 * Split pane splitting the container into
	 * upper and lower portions.
	 */
	private final JSplitPane split;
	
	/**
	 * Tabbed pane comprising the lower half
	 * of this complex split pane
	 */
	private final JTabbedPane tabs;
	
	/**
	 * Constructs a new complex split pane.
	 * 
	 * @param container Swing component for the pane
	 */
	public ComplexSplitPane(Container container)
	{
		split = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		
		container.setLayout(new BorderLayout());
		container.add(split, BorderLayout.CENTER);
		
		tabs = new JTabbedPane();
		split.setBottomComponent(tabs);
	}

	/**
	 * Specifies the top portion of the complex split pane.
	 * 
	 * @param component top portion component
	 */
	public void setTopComponent(Component component)
	{
		split.setTopComponent(component);
	}
	
	/**
	 * Adds a tab to the bottom portion of the
	 * complex split pane.
	 * 
	 * @param title     tab title
	 * @param component tab component
	 */
	public void addTab(String title, Component component)
	{
		tabs.addTab(title, component);
	}
	
}
