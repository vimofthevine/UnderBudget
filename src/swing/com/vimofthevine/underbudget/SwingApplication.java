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

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.UIManager;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.swing.AboutDialog;
import com.vimofthevine.underbudget.swing.ApplicationContent;
import com.vimofthevine.underbudget.swing.DeadEventListener;
import com.vimofthevine.underbudget.swing.PropertiesFileUserPreferences;
import com.vimofthevine.underbudget.swing.UserPreferences;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenu;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenuModel;
import com.vimofthevine.underbudget.swing.menu.ApplicationToolBar;
import com.vimofthevine.underbudget.swing.session.Sessions;
import com.vimofthevine.underbudget.swing.window.ApplicationWindow;
import com.vimofthevine.underbudget.swing.window.ApplicationWindowModel;

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
		
		new Sessions(frame, eventBus);
		new AboutDialog(frame, eventBus);
		new DeadEventListener(frame, eventBus);
		
		JPanel content = new JPanel();
		new ApplicationContent(content, eventBus);
		
		ApplicationWindowModel windowModel = new ApplicationWindowModel(eventBus, preferences);
		ApplicationWindow window = new ApplicationWindow(windowModel,
			frame, menuBar, toolBar, content);
		
		eventBus.register(preferences);
		eventBus.register(window);
		
		try {
			//UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) { }
		
		window.display();
	}

}
