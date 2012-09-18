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

package com.vimofthevine.underbudget.swing.transaction.wizard.csv;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.DefaultListSelectionModel;
import javax.swing.ListModel;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfile;
import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfileList;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class CsvProfileListViewModel {
	
	/**
	 * Profile list model
	 */
	private final ProfileListModel listModel;
	
	/**
	 * Profile selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Select-profile action model
	 */
	private final Action selectAction;
	
	/**
	 * Cancel-selection action model
	 */
	private final Action cancelAction;
	
	/**
	 * Currently selected CSV profile
	 */
	private CsvProfile profile;
	
	/**
	 * Constructs a new CSV profile list view model.
	 * 
	 * @param bus         event bus
	 * @param profileList CSV profile list
	 */
	CsvProfileListViewModel(final EventBus bus, CsvProfileList profileList)
	{
		listModel = new ProfileListModel(profileList.getProfileList());
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		selectionModel.addListSelectionListener(new ListSelectionListener() {
			@Override
            public void valueChanged(ListSelectionEvent event)
            {
				profile = listModel.getProfile(selectionModel.getLeadSelectionIndex());
				selectAction.setEnabled(true);
            }
		});
		
		selectAction = new AbstractAction() {
			{
				putValue(NAME, "Select");
				setEnabled(false);
			}

			@Override
            public void actionPerformed(ActionEvent e)
            {
				if (profile != null)
				{
					bus.post(new ProfileChosenEvent(profile));
					cancelAction.actionPerformed(null);
				}
            }
		};
		
		final Object listener = this;
		cancelAction = new AbstractAction() {
			{
				putValue(NAME, "Cancel");
			}
			
			@Override
			public void actionPerformed(ActionEvent e)
			{
				// Unregister so resources can be released
				bus.unregister(listener);
				bus.post(new ProfileChosenEvent(null));
			}
		};
	}
	
	/**
	 * Returns the available profile list model.
	 * 
	 * @return available profile list model
	 */
	ListModel getListModel()
	{
		return listModel;
	}
	
	/**
	 * Returns the profile selection model.
	 * 
	 * @return profile selection model
	 */
	ListSelectionModel getSelectionModel()
	{
		return selectionModel;
	}
	
	/**
	 * Returns the action model for selecting
	 * a profile to use for open a file.
	 * 
	 * @return select-profile action model
	 */
	Action getSelectAction()
	{
		return selectAction;
	}
	
	/**
	 * Returns the action model for cancelling
	 * the CSV profile selection.
	 * 
	 * @return cancel-selection action model
	 */
	Action getCancelAction()
	{
		return cancelAction;
	}

}
