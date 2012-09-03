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

package com.vimofthevine.underbudget.swing.estimate;

import java.awt.Component;
import java.util.Currency;
import java.util.Map;

import javax.swing.Action;
import javax.swing.ComboBoxModel;
import javax.swing.SwingUtilities;
import javax.swing.JToggleButton.ToggleButtonModel;
import javax.swing.text.Document;

import org.jdesktop.swingx.calendar.DateSelectionModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.currency.CurrencyInputModel;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;

/**
 * A presentation model for views that display
 * modifiable details about an individual estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDetailViewModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Estimate name document model
	 */
	private final NameModel nameDocument;
	
	/**
	 * Estimate description document model
	 */
	private final DescriptionModel descriptionDocument;
	
	/**
	 * Estimate amount document model
	 */
	private final EstimatedAmountModel amountDocument;
	
	/**
	 * Estimate type selection model
	 */
	private final EstimateTypeModel typeModel;
	
	/**
	 * Due date selection model
	 */
	private final DueDateModel dueDateModel;
	
	/**
	 * Estimate complete model
	 */
	private final EstimateCompleteModel completeModel;
	
	/**
	 * Add child estimate action
	 */
	private final AddChildAction addChildAction;
	
	/**
	 * Delete estimate action
	 */
	private final DeleteAction deleteAction;
	
	/**
	 * Currently selected estimate
	 */
	private MutableEstimate estimate;
	
	/**
	 * Constructs a new estimate detail view model.
	 * 
	 * @param bus      event bus
	 * @param currency currency in use
	 * @param parent  parent Swing component
	 */
	public EstimateDetailViewModel(EventBus bus, Currency currency,
		Component parent)
	{
		eventBus = bus;
		eventBus.register(this);
		
		nameDocument = new NameModel(bus);
		descriptionDocument = new DescriptionModel(bus);
		amountDocument = new EstimatedAmountModel(bus, currency);
		typeModel = new EstimateTypeModel(bus);
		dueDateModel = new DueDateModel(bus);
		completeModel = new EstimateCompleteModel(bus);
		
		addChildAction = new AddChildAction(bus);
		deleteAction = new DeleteAction(bus, parent);
	}
	
	/**
	 * Returns a document representing
	 * the estimate's user-defined name.
	 * 
	 * @return estimate name document
	 */
	Document getNameDocument()
	{
		return nameDocument;
	}

	/**
	 * Returns a document representing
	 * the estimate's user-defined description.
	 * 
	 * @return estimate name document
	 */
	Document getDescriptionDocument()
	{
		return descriptionDocument;
	}

	/**
	 * Returns a document representing
	 * the estimate's user-defined amount.
	 * 
	 * @return estimated amount document
	 */
	CurrencyInputModel getAmountModel()
	{
		return amountDocument;
	}
	
	/**
	 * Returns a combo-box model representing
	 * the type of the estimate.
	 * 
	 * @return estimate type combo box model
	 */
	ComboBoxModel getTypeModel()
	{
		return typeModel;
	}
	
	/**
	 * Returns a date selection model representing
	 * the due date of the estimate.
	 * 
	 * @return estimate due date selection model
	 */
	DateSelectionModel getDateModel()
	{
		return dueDateModel;
	}
	
	/**
	 * Returns the button model representing
	 * the estimate completeness (final) of
	 * the estimate.
	 * 
	 * @return estimate complete model
	 */
	ToggleButtonModel getCompleteModel()
	{
		return completeModel;
	}
	
	/**
	 * Returns an action for creating a new
	 * child estimate of the current estimate.
	 * 
	 * @return create child action
	 */
	Action getAddChildAction()
	{
		return addChildAction;
	}
	
	/**
	 * Returns an action for deleting the
	 * current estimate.
	 * 
	 * @return delete estimate action
	 */
	Action getDeleteAction()
	{
		return deleteAction;
	}
	
	/**
	 * Updates the input fields to reflect the
	 * currently selected estimate.
	 * 
	 * @param event estimate selection event
	 */
	@Subscribe
	public void estimateSelected(EstimateSelectedEvent event)
	{
		estimate = (MutableEstimate) event.getEstimate();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				nameDocument.setEstimate(estimate);
				descriptionDocument.setEstimate(estimate);
				amountDocument.setEstimate(estimate);
				typeModel.setEstimate(estimate);
				dueDateModel.setEstimate(estimate);
				completeModel.setEstimate(estimate);
				addChildAction.setEstimate(estimate);
				deleteAction.setEstimate(estimate);
			}
		});
	}
	
	@Subscribe
	public void estimateModified(EstimateModifiedEvent event)
	{
		if (event.getEstimate().equals(estimate))
		{
			final Map<String,String> changes = event.getChanges();
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
        			if (changes.containsKey("name"))
        			{
        				nameDocument.setEstimate(estimate);
        			}
        			if (changes.containsKey("description"))
        			{
        				descriptionDocument.setEstimate(estimate);
        			}
        			if (changes.containsKey("amount"))
        			{
       					amountDocument.setEstimate(estimate);
        			}
        			if (changes.containsKey("type"))
        			{
        				typeModel.setEstimate(estimate);
        			}
        			if (changes.containsKey("due-date"))
        			{
        				dueDateModel.setEstimate(estimate);
        			}
        			if (changes.containsKey("complete"))
        			{
        				completeModel.setEstimate(estimate);
        			}
				}
			});
		}
	}
	
}