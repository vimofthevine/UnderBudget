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

import java.awt.Component;

import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;

import com.vimofthevine.underbudget.core.currency.CashCommodity;

/**
 * Table cell renderer for displaying currency values.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencyCellRenderer extends DefaultTableCellRenderer {

	@Override
    public Component getTableCellRendererComponent(JTable table, Object value,
        boolean isSelected, boolean hasFocus, int row, int column)
    {
		super.getTableCellRendererComponent(table,
			value, isSelected, hasFocus, row, column);
		
		if (value instanceof CashCommodity)
		{
			setText(((CashCommodity) value).formatAsString());
		}
		
		return this;
    }

}
