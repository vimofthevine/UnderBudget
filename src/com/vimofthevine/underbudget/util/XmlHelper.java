package com.vimofthevine.underbudget.util;

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
	
}
