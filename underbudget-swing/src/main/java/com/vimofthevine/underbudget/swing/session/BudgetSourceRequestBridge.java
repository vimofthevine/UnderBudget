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

package com.vimofthevine.underbudget.swing.session;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.swing.session.events.SessionOpenedEvent;
import com.vimofthevine.underbudget.swing.session.source.SelectSource;

/**
 * A <code>SourceRequestBridge</code> forwards
 * budget source selection requests from a
 * session bus to the global bus.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class BudgetSourceRequestBridge {
	
	/**
	 * Application event bus
	 */
	private final EventBus globalBus;
	
	/**
	 * Constructs a new budget source selection
	 * request bridge.
	 * 
	 * @param globalBus  application event bus
	 * @param sessionBus session event bus
	 */
	BudgetSourceRequestBridge(EventBus globalBus, EventBus sessionBus)
	{
		this.globalBus = globalBus;
		sessionBus.register(this);
	}
	
	@Subscribe
	public void selectSource(SelectSource request)
	{
		globalBus.post(request);
	}
	
	@Subscribe
	public void sessionOpened(SessionOpenedEvent event)
	{
		globalBus.post(event);
	}

}
