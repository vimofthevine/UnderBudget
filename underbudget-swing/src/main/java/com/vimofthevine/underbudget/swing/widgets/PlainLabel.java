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

package com.vimofthevine.underbudget.swing.widgets;

import java.awt.Font;

import javax.swing.JLabel;

/**
 * A label whose font is plain.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class PlainLabel extends JLabel {

	/**
	 * Constructs a new plain label.
	 * 
	 * @param text label text
	 */
	public PlainLabel(String text)
	{
		super(text);
		setFont(getFont().deriveFont(Font.PLAIN));
	}

}
