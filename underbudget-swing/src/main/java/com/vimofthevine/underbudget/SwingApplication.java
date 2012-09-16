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

import java.io.File;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.UIManager;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.swing.AboutDialog;
import com.vimofthevine.underbudget.swing.ApplicationContent;
import com.vimofthevine.underbudget.swing.DeadEventListener;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenu;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenuModel;
import com.vimofthevine.underbudget.swing.menu.ApplicationToolBar;
import com.vimofthevine.underbudget.swing.preferences.NoUserPreferences;
import com.vimofthevine.underbudget.swing.preferences.PropertiesFileUserPreferences;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.Sessions;
import com.vimofthevine.underbudget.swing.tutorial.BasicUsageTutorialEvent;
import com.vimofthevine.underbudget.swing.tutorial.TutorialEventListener;
import com.vimofthevine.underbudget.swing.window.ApplicationWindow;
import com.vimofthevine.underbudget.swing.window.ApplicationWindowModel;

public class SwingApplication {
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		ApplicationProperties props = new ApplicationProperties();
		UserPreferences preferences;
		
		// Figure out location of app preferences
		String home = System.getProperty("user.home");
		File prefsHome = new File(home + "/.config/underbudget");
		
		// If the prefs directories doesn't exist, this is the
		// first time the application is being executed
		boolean firstExecution = ! prefsHome.exists();
		// If first execution, create the prefs directory
		if (firstExecution)
		{
			prefsHome.mkdir();
		}
		
		// If prefs home doesn't exist, use no-prefs
		preferences = ( ! prefsHome.exists())
			? new NoUserPreferences()
			: new PropertiesFileUserPreferences(props,
				prefsHome.getPath() + "/prefs.properties");
		preferences.read();
		
		// Store preferences dir so application can get it
		preferences.set("HOME", prefsHome.getPath());
		
		EventBus eventBus = new EventBus("main");
		
		JFrame frame = new JFrame();
		
		ApplicationMenuModel menuModel = new ApplicationMenuModel(eventBus);
		JMenuBar menuBar = new JMenuBar();
		new ApplicationMenu(menuModel, menuBar);
		JToolBar toolBar = new JToolBar(props.getTitle());
		new ApplicationToolBar(menuModel, toolBar);
		
		new Sessions(frame, eventBus, preferences);
		new AboutDialog(props, frame, eventBus);
		new TutorialEventListener(frame, eventBus);
		new DeadEventListener(frame, eventBus);
		
		JPanel content = new JPanel();
		new ApplicationContent(props, content, eventBus);
		
		ApplicationWindowModel windowModel =
			new ApplicationWindowModel(props, eventBus, preferences);
		ApplicationWindow window = new ApplicationWindow(windowModel,
			frame, menuBar, toolBar, content);
		
		eventBus.register(preferences);
		eventBus.register(window);
		
		try {
			//UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) { }
		
		window.display();
		
		if (firstExecution)
		{
			eventBus.post(new BasicUsageTutorialEvent());
		}
	}

}
