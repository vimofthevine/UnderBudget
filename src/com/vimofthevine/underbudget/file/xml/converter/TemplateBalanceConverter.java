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

package com.vimofthevine.underbudget.file.xml.converter;

import org.simpleframework.xml.convert.Converter;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.OutputNode;

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * Converter to remove the initial balance from a
 * template budget when serializing.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TemplateBalanceConverter implements Converter<Currency> {

	@Override
    public Currency read(InputNode arg0) throws Exception
    {
		throw new UnsupportedOperationException("Cannot use template balance converter for deserialization");
    }

	@Override
    public void write(OutputNode arg0, Currency arg1) throws Exception
    {
		arg0.remove();
    }

}
