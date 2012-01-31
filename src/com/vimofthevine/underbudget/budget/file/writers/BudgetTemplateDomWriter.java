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

package com.vimofthevine.underbudget.budget.file.writers;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.util.XmlHelper;

/**
 * Budget template writer using the DOM XML API
 * 
 * This class extends the default budget file writer since
 * there are only a few minor differences between normal
 * budget files and a template. When saving a budget as a
 * template, all estimates are left as NOT final, no initial
 * balance is recorded, and the name is reset.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetTemplateDomWriter extends BudgetFileDomWriter {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetTemplateDomWriter.class.getName());
	
	/**
	 * Creates a meta element with out any of the budget info
	 */
	@Override
	protected Element createMetaElement(Budget budget, float allocated)
	{
		logger.log(Level.FINE, "Creating template meta element");
		
		Element metaElement = doc.createElement("meta");
		
		XmlHelper.createElement(doc, metaElement, "name", "New Budget");
		
		progress.add(allocated);
		
		return metaElement;
	}
	
	/**
	 * Populates an estimate element, keeping the estimate as non-final
	 */
	@Override
	protected void populateEstimateMeta(Estimate estimate, Element element)
	{
		XmlHelper.createElement(doc, element, "name", estimate.getName());
		XmlHelper.createElement(doc, element, "notes", estimate.getNotes());
		
		if ( ! estimate.isCategory())
		{
			XmlHelper.createElement(doc, element, "amount", String.valueOf(estimate.getAmount()));
			XmlHelper.createElement(doc, element, "final", "false");
		}
	}
	
}
