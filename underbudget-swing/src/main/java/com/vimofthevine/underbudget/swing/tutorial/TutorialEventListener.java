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

import java.awt.Frame;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;

/**
 * Listener to respond to tutorial events and
 * display a tutorial.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TutorialEventListener {
	
	/**
	 * Application window
	 */
	private final Frame parent;
	
	/**
	 * Constructs a new tutorial event listener.
	 * 
	 * @param window application window
	 * @param bus    event bus
	 */
	public TutorialEventListener(Frame window, EventBus bus)
	{
		bus.register(this);
		
		parent = window;
	}
	
	/**
	 * Displays a basic usage tutorial.
	 * 
	 * @param event basic-usage tutorial event
	 */
	@Subscribe
	public void showBasicUsageTutorial(BasicUsageTutorialEvent event)
	{
		new BasicUsageTutorial(parent);
	}

}
