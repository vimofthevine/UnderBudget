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

package com.vimofthevine.underbudget.swing.currency;

import javax.swing.InputVerifier;
import javax.swing.JComponent;
import javax.swing.JTextField;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

/**
 * Input verifier to verify that a text field contains
 * a valid currency amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class CurrencyInputVerifier extends InputVerifier {
	
	/**
	 * Mathematical calculator
	 */
	private final Calculator calculator = new Calculator();

	/**
	 * Verifies that the input of a text field
	 * contains a valid currency amount.
	 * 
	 * @see javax.swing.InputVerifier#verify(javax.swing.JComponent)
	 */
	@Override
	public boolean verify(JComponent arg0)
	{
		if (arg0 instanceof JTextField)
		{
			JTextField field = (JTextField) arg0;
			Document document = field.getDocument();
			
			try
			{
				String amount = document.getText(0, document.getLength());
				return (calculator.calculate(amount) != null);
			}
			catch (NumberFormatException nfe)
			{
				return false;
			}
			catch (BadLocationException ble)
			{
				return false;
			}
		}
		
		return false;
	}

}
