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

package com.vimofthevine.underbudget.file.xml;

import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.convert.AnnotationStrategy;
import org.simpleframework.xml.convert.Registry;
import org.simpleframework.xml.convert.RegistryStrategy;
import org.simpleframework.xml.core.Persister;
import org.simpleframework.xml.strategy.Strategy;
import org.simpleframework.xml.strategy.VisitorStrategy;
import org.simpleframework.xml.stream.Format;

import com.vimofthevine.underbudget.file.xml.converter.EstimateFinalConverter;
import com.vimofthevine.underbudget.file.xml.strategy.BalanceImpactVisitor;
import com.vimofthevine.underbudget.file.xml.strategy.BudgetingPeriodVisitor;
import com.vimofthevine.underbudget.file.xml.strategy.CurrencyVisitor;
import com.vimofthevine.underbudget.file.xml.strategy.EstimateBehaviorVisitor;
import com.vimofthevine.underbudget.file.xml.strategy.EstimateReferenceStrategy;

/**
 * Utility for creating and configuring a budget
 * serializer using the Simple xml framework
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetSerializerFactory {
	
	public static final String PROLOG = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";

	/**
	 * Creates a new serializer, configuring it as needed
	 * 
	 * @return budget serializer
	 */
	public static Serializer createSerializer()
	{
		return createSerializer(false);
	}
	
	/**
	 * Creates a new serializer, configuring it as
	 * needed to produce template budgets
	 * 
	 * @return template budget serializer
	 */
	public static Serializer createTemplateSerializer()
	{
		return createSerializer(true);
	}
	
	/**
	 * Creates a serializer, configuring it as needed
	 * 
	 * @param template if true the serializer will be
	 *                  configured to produce/read template budgets
	 * @return budget serializer
	 */
	private static Serializer createSerializer(boolean template)
	{
		// Give id and reference attributes to link estimates and rules
		EstimateReferenceStrategy referenceStrategy = new EstimateReferenceStrategy();
		// Specify budgeting period type as an attribute
		VisitorStrategy periodStrategy = new VisitorStrategy(new BudgetingPeriodVisitor(), referenceStrategy);
		// Specify currency type as an attribute
		VisitorStrategy currencyStrategy = new VisitorStrategy(new CurrencyVisitor(), periodStrategy);
		// Specify impact type as the element value
		VisitorStrategy impactStrategy = new VisitorStrategy(new BalanceImpactVisitor(), currencyStrategy);
		// Specify behavior type as an attribute
		VisitorStrategy behaviorStrategy = new VisitorStrategy(new EstimateBehaviorVisitor(), impactStrategy);
		
		Registry registry = new Registry();
		RegistryStrategy registryStrategy = new RegistryStrategy(registry, behaviorStrategy);
		
		try
		{
			// Clean up final elements
			registry.bind(Boolean.class, new EstimateFinalConverter(template));
		}
		catch (Exception e) { }
		
		Format format = new Format(PROLOG);
		
		Strategy strategy = template ? new AnnotationStrategy(registryStrategy) : registryStrategy;
		return new Persister(strategy, new BudgetMatcher(), format);
	}
	
}
