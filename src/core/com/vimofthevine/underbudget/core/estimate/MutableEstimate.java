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

package com.vimofthevine.underbudget.core.estimate;

import java.util.Date;

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * An estimate whose definition can be modified.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface MutableEstimate extends Estimate {
	
	/**
	 * Updates this estimate's definition.
	 * 
	 * @param definition new estimate definition
	 */
	public void setDefinition(EstimateDefinition definition);
	
	/**
	 * Creates a new child estimate to this estimate.
	 * 
	 * @return new child estimate
	 */
	public Estimate createChild();
	
	/**
	 * Deletes this estimate, removing itself from
	 * its parent estimate.
	 */
	public void delete();
	
	@Deprecated
	public void setName(String name);
	
	@Deprecated
	public void setDescription(String description);
	
	@Deprecated
	public void setAmount(Currency amount);
	
	@Deprecated
	public void setDueDate(Date dueDate);
	
	@Deprecated
	public void setType(EstimateType type);
	
	@Deprecated
	public void setComplete(boolean complete);
	
}
