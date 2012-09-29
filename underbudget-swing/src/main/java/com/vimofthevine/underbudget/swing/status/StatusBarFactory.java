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

package com.vimofthevine.underbudget.swing.status;

import java.awt.Component;

import javax.swing.JPanel;

import com.google.common.eventbus.EventBus;

/**
 * Factory for creating status bar view instances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class StatusBarFactory {

	/**
	 * Constructs a new status bar view instance.
	 * 
	 * @param bus event bus
	 * @return status bar view
	 */
	public static Component build(EventBus bus)
	{
		StatusBarViewModel model = new StatusBarViewModel(bus);
		JPanel view = new JPanel();
		new StatusBarView(view, model);
		return view;
	}

}
