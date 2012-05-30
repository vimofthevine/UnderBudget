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

import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;

/**
 * Custom transformer for transaction field enumerations that
 * supports loose translation (case insensitive)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFieldTransform implements Transform<TransactionField> {

	/**
	 * Converts the given string value to an appropriate
	 * TransactionField enumeration. This is used when an object is
	 * being deserialized from the XML document and the value for
	 * the string representation is required.
	 * 
	 * @param value string representation of the value
	 * @return TransactionField enumeration represented by the string
	 * @see TransactionField.getValueOf()
	 */
	@Override
    public TransactionField read(String value) throws Exception
    {
		return TransactionField.getValueOf(value);
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
    public String write(TransactionField value) throws Exception
    {
		return value.name().toLowerCase();
    }

}
