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

package com.vimofthevine.underbudget.xml.assignment;

import java.util.ArrayList;

import com.vimofthevine.underbudget.core.assignment.Condition;

/**
 * Utility class for dealing with sets of XML conditions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
abstract class XmlConditions {
	
	/**
	 * Convert the given set of conditions to
	 * XML serializable conditions.
	 * 
	 * @param conditions original conditions
	 * @return set of XML serializable conditions
	 */
	public static XmlCondition[] toArray(Condition[] conditions)
	{
		XmlCondition[] xmlConditions = new XmlCondition[conditions.length];
		
		for (int i=0; i<conditions.length; i++)
		{
			Condition condition = conditions[i];
			xmlConditions[i] = new XmlCondition(condition);
		}
		
		return xmlConditions;
	}
	
	/**
	 * Convert the given set of conditions to
	 * XML serializable conditions.
	 * 
	 * @param conditions original conditions
	 * @return list of XML serializable conditions
	 */
	public static ArrayList<XmlCondition> toList(Condition[] conditions)
	{
		ArrayList<XmlCondition> xmlConditions = new ArrayList<XmlCondition>();
		
		for (int i=0; i<conditions.length; i++)
		{
			Condition condition = conditions[i];
			xmlConditions.add(i, new XmlCondition(condition));
		}
		
		return xmlConditions;
	}

}
