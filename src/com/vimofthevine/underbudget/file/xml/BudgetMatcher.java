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

import org.simpleframework.xml.transform.Matcher;
import org.simpleframework.xml.transform.Transform;

import com.vimofthevine.underbudget.core.budget.assignment.ComparisonOperator;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionField;
import com.vimofthevine.underbudget.file.xml.transform.ComparisonOperatorTransform;
import com.vimofthevine.underbudget.file.xml.transform.TransactionFieldTransform;

/**
 * Custom matcher to support custom transforms when
 * serializing/deserializing budget definitions to/from
 * XML.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMatcher implements Matcher {

	@Override
    public Transform<?> match(Class clazz) throws Exception
    {
		if (clazz.equals(TransactionField.class))
			return new TransactionFieldTransform();
		else if (clazz.equals(ComparisonOperator.class))
			return new ComparisonOperatorTransform();
		else
			return null;
    }

}
