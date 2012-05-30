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

import java.util.Calendar;
import java.util.Date;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 * XML helper utility
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlHelper {

	/**
	 * Convenience method for getting a text node from an
	 * element, given the tag name of the node.
	 * 
	 * @param element element within which to perform the search
	 * @param tag     name of the node for which to search
	 * @return contents of the found node
	 */
	public static String readTextNode(Element element, String tag)
	{
		if (((NodeList) element.getElementsByTagName(tag)).getLength() > 0)
		{
			Element tagElement = (Element) element.getElementsByTagName(tag).item(0);
			if (tagElement.hasChildNodes())
				return tagElement.getFirstChild().getNodeValue();
			else
				return "";
		}
		else
			return "";
	}
	
	/**
	 * Convenience method for creating a new XML element as
	 * a child element of another element
	 * 
	 * @param doc      DOM document model in use
	 * @param parent   parent element
	 * @param tag      new element tag name
	 * @param contents new element contents
	 * @return the new element
	 */
	public static Element createElement(Document doc, Element parent,
		String tag, Object contents)
	{
		Element newElement = doc.createElement(tag);
		
		if (contents != null)
		{
			newElement.appendChild(doc.createTextNode(String.valueOf(contents)));
		}
		
		parent.appendChild(newElement);
		
		return newElement;
	}
	
	/**
	 * Convenience method for populating a given element as a
	 * complex date element, with child year, month, and date
	 * elements
	 */
	public static void createDateElement(Document doc, Element parent, Date date)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		
		createElement(doc, parent, "year",
			String.valueOf(calendar.get(Calendar.YEAR)));
		createElement(doc, parent, "month",
			String.valueOf(calendar.get(Calendar.MONTH)));
		createElement(doc, parent, "date",
			String.valueOf(calendar.get(Calendar.DAY_OF_MONTH)));
	}
	
	/**
	 * Convenience method for reading a date out of
	 * a nested DOM element
	 * 
	 * @param element parent element for the date
	 * @return date represented in the given DOM element
	 */
	public static Date readDateElement(Element element)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.clear();
		
		calendar.set(Calendar.YEAR, Integer.valueOf(readTextNode(element, "year")));
		calendar.set(Calendar.MONTH, Integer.valueOf(readTextNode(element, "month")));
		calendar.set(Calendar.DAY_OF_MONTH, Integer.valueOf(readTextNode(element, "date")));
		
		return calendar.getTime();
	}
	
}
