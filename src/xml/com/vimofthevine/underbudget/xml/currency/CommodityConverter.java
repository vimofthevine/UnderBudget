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

package com.vimofthevine.underbudget.xml.currency;

import org.simpleframework.xml.convert.Converter;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.OutputNode;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;

/**
 * Converter to serialize/deserialize cash commodity values.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CommodityConverter implements Converter<CashCommodity> {
	
	/**
	 * Currency attribute
	 */
	private static final String CURRENCY = "currency";

	@Override
    public CashCommodity read(InputNode node) throws Exception
    {
		String iso4217 = node.getAttribute(CURRENCY).getValue();
		String amount = node.getValue();
		return Commodity.create(iso4217, amount);
    }

	@Override
    public void write(OutputNode node, CashCommodity value) throws Exception
    {
		String iso4217 = value.getCurrency().getCurrencyCode();
		String amount = value.getValue().asString();
		
		node.getAttributes().remove("class");
		node.setAttribute(CURRENCY, iso4217);
		node.setValue(amount);
    }

}
