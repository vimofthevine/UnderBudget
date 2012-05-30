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

package com.vimofthevine.underbudget.file.xml.transform;

import org.simpleframework.xml.transform.Transform;

import com.vimofthevine.underbudget.core.budget.assignment.ComparisonOperator;

/**
 * Custom transformer for comparison operator enumerations that
 * supports loose translation (case insensitive)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ComparisonOperatorTransform implements Transform<ComparisonOperator> {

	/**
	 * Converts the given string value to an appropriate
	 * ComparisonOperator enumeration. This is used when an object is
	 * being deserialized from the XML document and the value for
	 * the string representation is required.
	 * 
	 * @param value string representation of the value
	 * @return ComparisonOperator enumeration represented by the string
	 * @see ComparisonOperator.getValueOf()
	 */
	@Override
    public ComparisonOperator read(String value) throws Exception
    {
		return ComparisonOperator.getValueOf(value);
    }

	/**
	 * Converts the provided value into an XML usable format.
	 * This is used in the serialization process when there
	 * is a need to convert a field value into a string so
	 * that the value can be written as a valid XML entity.
	 * 
	 * @param value enumeration to be converted to a string
	 * @return string representation of the given value
	 */
	@Override
    public String write(ComparisonOperator value) throws Exception
    {
		return value.name().toLowerCase();
    }
}
