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
import java.awt.Component;
import java.util.HashMap;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.UIManager;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.analysis.ActualFigure;
import com.vimofthevine.underbudget.core.analysis.ActualFigureSource;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.estimate.DefaultEstimate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.AboutDialog;
import com.vimofthevine.underbudget.swing.PropertiesFileUserPreferences;
import com.vimofthevine.underbudget.swing.UserPreferences;
import com.vimofthevine.underbudget.swing.content.ContentDialog;
import com.vimofthevine.underbudget.swing.content.ContentDisplay;
import com.vimofthevine.underbudget.swing.content.ContentView;
import com.vimofthevine.underbudget.swing.content.ContentViewModel;
import com.vimofthevine.underbudget.swing.content.DefaultContentView;
import com.vimofthevine.underbudget.swing.estimate.BalanceImpactViewFactory;
import com.vimofthevine.underbudget.swing.estimate.EstimateProgressViewFactory;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenu;
import com.vimofthevine.underbudget.swing.menu.ApplicationMenuModel;
import com.vimofthevine.underbudget.swing.menu.ApplicationToolBar;
import com.vimofthevine.underbudget.swing.status.StatusBar;
import com.vimofthevine.underbudget.swing.status.StatusBarModel;
import com.vimofthevine.underbudget.swing.window.ApplicationWindow;
import com.vimofthevine.underbudget.swing.window.ApplicationWindowModel;

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
		
		final CurrencyFactory factory = new CurrencyFactory("USD");
		
		JFrame frame = new JFrame();
		
		ApplicationMenuModel menuModel = new ApplicationMenuModel(eventBus);
		JMenuBar menuBar = new JMenuBar();
		new ApplicationMenu(menuModel, menuBar);
		JToolBar toolBar = new JToolBar(Application.TITLE);
		new ApplicationToolBar(menuModel, toolBar);
		
		ActualFigureSource actuals = new ActualFigureSource() {
			@Override
            public ActualFigure getActualFigure(Estimate estimate)
            {
				return new ActualFigure() {
					public Currency getAmount()
					{
						return factory.newCurrencyInstance("20.25");
					}
				};
            }
		};
		
		Estimate root = createEstimateTree();
		
		Component progress = EstimateProgressViewFactory.build(
			frame, eventBus, factory, root, actuals);
		Component impact = BalanceImpactViewFactory.build(
			frame, eventBus, factory, root, actuals);
		
		HashMap<ContentDisplay, ContentDialog> dialogs = new HashMap<ContentDisplay, ContentDialog>();
		dialogs.put(ContentDisplay.ABOUT, new AboutDialog(frame));
		
		JPanel content = new JPanel();
		ContentView contentView = new DefaultContentView(content);
		contentView.addDisplay(new JLabel("Assignment Rules"), ContentDisplay.ASSIGNMENT_RULES);
		contentView.addDisplay(impact, ContentDisplay.BALANCE_IMPACT);
		contentView.addDisplay(progress, ContentDisplay.ESTIMATE_PROGRESS);
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
		ApplicationWindow window = new ApplicationWindow(windowModel,
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
