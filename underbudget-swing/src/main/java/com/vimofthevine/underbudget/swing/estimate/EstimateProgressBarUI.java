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

package com.vimofthevine.underbudget.swing.estimate;

import java.awt.Color;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;

import javax.swing.JComponent;
import javax.swing.plaf.basic.BasicProgressBarUI;

/**
 * Custom progress bar look-and-feel.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimateProgressBarUI extends BasicProgressBarUI {
	
	/**
	 * Set antialiasing.
	 */
	@Override
	public void paint(Graphics g, JComponent c)
	{
		Graphics2D g2 = (Graphics2D) g;
		
		// Geometric shapes
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
			RenderingHints.VALUE_ANTIALIAS_ON);
		
		// Text
		g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
			RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
		
		// Quality over speed
		g2.setRenderingHint(RenderingHints.KEY_RENDERING,
			RenderingHints.VALUE_RENDER_QUALITY);
		
		super.paint(g, c);
	}
	
	/**
	 * Draw the progress bar as a rounded rectange
	 * and with a gradient.
	 */
	@Override
	protected void paintDeterminate(Graphics g, JComponent c)
	{
		Insets insets = progressBar.getInsets();
		int width = progressBar.getWidth();
		int height = progressBar.getHeight();
		int barRectWidth = width - (insets.right + insets.left);
		int barRectHeight = height - (insets.top + insets.bottom);
		int arcSize = 5;
		int amountFull = getAmountFull(insets, barRectWidth, barRectHeight);
		
		Graphics2D g2 = (Graphics2D) g;
		g2.setColor(progressBar.getBackground());
		g2.fillRoundRect(0, 0, width-1, height-1, arcSize, arcSize);
		
		// Create gradient
		Color color = progressBar.getForeground();
		Color lighter = color.brighter();
		//Color darker = color.darker();
		GradientPaint gradient = new GradientPaint(width/2, 0, lighter,
			width/2, height/3, color, false);
		
		g2.setPaint(gradient);
		g2.fillRoundRect(insets.left, insets.top, amountFull-1,
			barRectHeight-1, arcSize, arcSize);
		
		if (progressBar.isStringPainted())
		{
			paintString(g, insets.left, insets.top,
				barRectWidth, barRectHeight, amountFull, insets);
		}
	}

}
