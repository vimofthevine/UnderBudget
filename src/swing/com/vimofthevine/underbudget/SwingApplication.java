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
import java.awt.Color;
import java.util.HashMap;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.UIManager;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.analysis.Actual;
import com.vimofthevine.underbudget.analysis.ActualsCache;
import com.vimofthevine.underbudget.currency.Currency;
import com.vimofthevine.underbudget.currency.CurrencyFactory;
import com.vimofthevine.underbudget.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.estimate.Estimate;
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
import com.vimofthevine.underbudget.gui.estimate.EstimateProgressTreeTableModel;
import com.vimofthevine.underbudget.gui.estimate.EstimateProgressView;
import com.vimofthevine.underbudget.gui.estimate.EstimateProgressViewModel;
import com.vimofthevine.underbudget.gui.menu.ApplicationMenu;
import com.vimofthevine.underbudget.gui.menu.ApplicationMenuModel;
import com.vimofthevine.underbudget.gui.menu.ApplicationToolBar;
import com.vimofthevine.underbudget.gui.status.StatusBar;
import com.vimofthevine.underbudget.gui.status.StatusBarModel;

public class SwingApplication {
	
	private static Estimate createEstimateTree()
	{
		CurrencyFactory factory = new CurrencyFactory("USD");
		DefaultEstimate root = new DefaultEstimate("Root", factory.newCurrencyInstance());
		DefaultEstimate cat1 = new DefaultEstimate("Category 1", factory.newCurrencyInstance());
		DefaultEstimate cat2 = new DefaultEstimate("Category 2", factory.newCurrencyInstance());
		DefaultEstimate est1 = new DefaultEstimate("Estimate 1", factory.newCurrencyInstance("12.34"));
		DefaultEstimate est2 = new DefaultEstimate("Estimate 2", factory.newCurrencyInstance("100.00"));
		DefaultEstimate est3 = new DefaultEstimate("Estimate 3", factory.newCurrencyInstance("75.52"));
		DefaultEstimate est4 = new DefaultEstimate("Estimate 4", factory.newCurrencyInstance("30.00"));
		DefaultEstimate est5 = new DefaultEstimate("Estimate 5", factory.newCurrencyInstance("75.00"));
		
		root.add(cat1);
		root.add(cat2);
		cat1.add(est1);
		cat1.add(est2);
		cat2.add(est3);
		cat2.add(est4);
		root.add(est5);
		
		return root;
	}

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
		
		EstimateProgressTreeTableModel treeTableModel = new EstimateProgressTreeTableModel(
			createEstimateTree(), new ActualsCache() {
				private CurrencyFactory factory = new CurrencyFactory("USD");
				
				@Override
                public Actual getActual(Estimate estimate)
                {
					return new Actual() {
						public Currency getAmount()
						{
							return factory.newCurrencyInstance("20.25");
						}
					};
                }
				
			});
		
		EstimateProgressViewModel estimateProgressModel = new EstimateProgressViewModel(treeTableModel, null);
		JPanel estimateProgress = new JPanel();
		new EstimateProgressView(estimateProgressModel, estimateProgress);
		
		HashMap<ContentDisplay, ContentDialog> dialogs = new HashMap<ContentDisplay, ContentDialog>();
		dialogs.put(ContentDisplay.ABOUT, new AboutDialog(frame));
		
		JPanel content = new JPanel();
		ContentView contentView = new DefaultContentView(content);
		contentView.addDisplay(new JLabel("Assignment Rules"), ContentDisplay.ASSIGNMENT_RULES);
		contentView.addDisplay(new JLabel("Balance Impact"), ContentDisplay.BALANCE_IMPACT);
		contentView.addDisplay(estimateProgress, ContentDisplay.ESTIMATE_PROGRESS);
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
		
		try {
			//UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) { }
		
		window.display();
	}

}
