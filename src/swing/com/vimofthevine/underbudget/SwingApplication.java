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

package com.vimofthevine.underbudget;

import java.awt.BorderLayout;
import java.util.HashMap;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JToolBar;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.gui.AboutDialog;
import com.vimofthevine.underbudget.gui.ApplicationWindow;
import com.vimofthevine.underbudget.gui.ApplicationWindowModel;
import com.vimofthevine.underbudget.gui.DefaultApplicationWindow;
import com.vimofthevine.underbudget.gui.PropertiesFileUserPreferences;
import com.vimofthevine.underbudget.gui.UserPreferences;
import com.vimofthevine.underbudget.gui.content.ContentDialog;
import com.vimofthevine.underbudget.gui.content.ContentDisplay;
import com.vimofthevine.underbudget.gui.content.ContentView;
import com.vimofthevine.underbudget.gui.content.ContentViewModel;
import com.vimofthevine.underbudget.gui.content.DefaultContentView;
import com.vimofthevine.underbudget.gui.menu.ApplicationMenu;
import com.vimofthevine.underbudget.gui.menu.ApplicationMenuModel;
import com.vimofthevine.underbudget.gui.menu.ApplicationToolBar;
import com.vimofthevine.underbudget.gui.status.StatusBar;
import com.vimofthevine.underbudget.gui.status.StatusBarModel;

public class SwingApplication {

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		EventBus eventBus = new EventBus("main");
		UserPreferences preferences = new PropertiesFileUserPreferences("/tmp/underbudget.properties");
		preferences.read();
		
		JFrame frame = new JFrame();
		
		ApplicationMenuModel menuModel = new ApplicationMenuModel(eventBus);
		JMenuBar menuBar = new JMenuBar();
		new ApplicationMenu(menuModel, menuBar);
		JToolBar toolBar = new JToolBar(Application.TITLE);
		new ApplicationToolBar(menuModel, toolBar);
		
		HashMap<String, ContentDialog> dialogs = new HashMap<String, ContentDialog>();
		dialogs.put(ContentDisplay.ABOUT.toString(), new AboutDialog(frame));
		
		JPanel content = new JPanel();
		ContentView contentView = new DefaultContentView(content);
		contentView.addDisplay(new JLabel("Assignment Rules"), ContentDisplay.ASSIGNMENT_RULES);
		contentView.addDisplay(new JLabel("Balance Impact"), ContentDisplay.BALANCE_IMPACT);
		contentView.addDisplay(new JLabel("Estimate Progress"), ContentDisplay.ESTIMATE_PROGRESS);
		contentView.addDisplay(new JLabel("Imported Transactions"), ContentDisplay.IMPORTED_TRANSACTIONS);
		ContentViewModel contentModel = new ContentViewModel(contentView, dialogs);
		
		JPanel statusBar = new JPanel();
		statusBar.setBorder(BorderFactory.createEmptyBorder(0, 2, 2, 2));
		StatusBar status = new StatusBar(statusBar);
		StatusBarModel statusModel = new StatusBarModel(status);
		
		JPanel composite = new JPanel(new BorderLayout());
		composite.add(content, BorderLayout.CENTER);
		composite.add(statusBar, BorderLayout.SOUTH);
		
		ApplicationWindowModel windowModel = new ApplicationWindowModel(eventBus, preferences);
		ApplicationWindow window = new DefaultApplicationWindow(windowModel,
			frame, menuBar, toolBar, composite);
		
		eventBus.register(preferences);
		eventBus.register(window);
		eventBus.register(statusModel);
		eventBus.register(contentModel);
		
		window.display();
	}

}
