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

package com.vimofthevine.underbudget.budget.period;

import java.util.Date;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 * Interface for budgeting period representations
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetingPeriod {

	/**
	 * Returns the start date for the
	 * budgeting period
	 * 
	 * @return budgeting period start date
	 */
	public Date getStartDate();
	
	/**
	 * Returns the end date for the
	 * budgeting period
	 * 
	 * @return budgeting period end date
	 */
	public Date getEndDate();
	
	/**
	 * Stores the budgeting period as XML elements
	 * of the given XML element
	 * 
	 * @param doc     DOM document to use
	 * @param element XML element in which to store
	 */
	public void store(Document doc, Element element);
	
	/**
	 * Parses the budgeting period from a given XML element
	 * 
	 * @param element budgeting period XML element
	 * @return the budgeting period defined by the XML,
	 *         if the XML defines a valid budgeting period
	 */
	public BudgetingPeriod parse(Element element);
	
}
