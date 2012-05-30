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
 * Converter to clean up the output of the estimate final
 * boolean value. Additionally, it can remove the final
 * field for all estimates, such as when serializing for
 * a template.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateFinalConverter implements Converter<Boolean> {
	
	/**
	 * Whether serializing/deserializing as a template
	 */
	private final boolean template;
	
	/**
	 * Constructs a new estimate final field converter.
	 * 
	 * @param template true if serializing as a template
	 */
	public EstimateFinalConverter(boolean template)
	{
		this.template = template;
	}

	@Override
    public Boolean read(InputNode node) throws Exception
    {
		if (node.getName().equals("final"))
		{
    		// All estimates are not final in a template
    		if (template)
    			return false;
    		// If the final element is present the estimate is final
    		else
    			return true;
		}
		else
			return Boolean.valueOf(node.getValue());
    }

	@Override
    public void write(OutputNode node, Boolean value) throws Exception
    {
		if (node.getName().equals("final"))
		{	
    		// All estimates are not final in a template
    		if (template)
    		{
    			node.remove();
    		}
    		// If it is not final, don't put any element in the XML
    		else if ( ! value)
    		{
    			node.remove();
    		}
    		// Else clear out the value so it will be printed as
    		// <final/> instead of <final>true</final>
    		else
    		{
    			node.setValue(null);
    		}
		}
    }

}
