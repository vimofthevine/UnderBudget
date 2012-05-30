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

/**
 * Converter to deserialize empty strings as "" instead
 * of null.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EmptyStringConverter implements Converter<String> {

	@Override
    public String read(InputNode node) throws Exception
    {
		if (node.isEmpty())
			return "";
		
		return node.getValue();
    }

	@Override
    public void write(OutputNode node, String value) throws Exception
    {
		node.setValue(value);
    }

}
