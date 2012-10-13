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

/**
 * Mathematical expression calculator.
 * <p>
 * Expressions are broken up and evaluated as follows:
 * <ul>
 *   <li>Expression : term {+,-} term</li>
 *   <li>Term : factor {*,/} factor</li>
 *   <li>Factor : (-) {number,(expression)}</li>
 * </ul>
 * <p>
 * The <strong>expression</strong> is evaluated as a
 * <strong>term</strong>, a + or - operator (if any)
 * and another <strong>term</strong>, a + or - operator
 * (if any) and another <strong>term</strong>, etc.
 * <p>
 * Each <strong>term</strong> is evaluated as a
 * <strong>factor</strong>, a * or / operator
 * (if any) and another <strong>factor</strong>,
 * a * or / operator (if any) and another <strong>factor</strong>, etc.
 * <p>
 * Each <strong>factor</strong> is evaluated as either a
 * sub-expression (a set of parenthesis) or a
 * <strong>number</strong>, which may be negative or
 * positive. Sub-expressions are evaluated as an
 * <strong>expression</strong>, introducing recursion.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class Calculator {
	
	/**
	 * Calculates the value of the given expression.
	 * 
	 * @param expression expression to be evaluated
	 * @return value of the expression, or <code>null</code>
	 *         if the expression is invalid
	 */
	Float calculate(String expression)
	{
		return evaluateExpression(expression, new IntPtr(0));
	}
	
	/**
	 * Evaluates the value of the sub-expression in the given
	 * expression at the given position.
	 * 
	 * @param expr expression
	 * @param pos  position in the expression
	 * @return value of the expression
	 */
	private float evaluateExpression(String expr, IntPtr pos)
	{
		// Get next term in the expression
		Float result = evaluateTerm(expr, pos);
		
		while (pos.value < expr.length())
		{
			// Get operator
			char op = expr.charAt(pos.value);
			// If not addition or subtraction, return term
			if (op != '+' && op != '-')
				return result;
			pos.increment();
			
			// Get next term
			Float term = evaluateTerm(expr, pos);
			// If two valid terms
			if (term != null && result != null)
			{
				if (op == '+')
				{
					result = result + term;
				}
				else
				{
					result = result - term;
				}
			}
			// One term is invalid
			else
			{
				result = null;
			}
		}
		
		return result;
	}
	
	/**
	 * Evaluates the value of the term located in the
	 * given expression at the given position.
	 * 
	 * @param expr expression
	 * @param pos  position in the expression
	 * @return value of the term
	 */
	private float evaluateTerm(String expr, IntPtr pos)
	{
		// Get next factor in the expression
		Float result = evaluateFactor(expr, pos);
		
		while (pos.value < expr.length())
		{
			// Get operator
			char op = expr.charAt(pos.value);
			// If not multiplication or division, return factor
			if (op != '*' && op != '/')
				return result;
			pos.increment();
			
			// Get next factor
			Float factor = evaluateFactor(expr, pos);
			// If two valid factors
			if (factor != null && result != null)
			{
				// Multiplication
				if (op == '*')
				{
					result = result * factor;
				}
				// Division
				else
				{
					// Don't divide by 0
					if (factor.compareTo(0.0f) == 0)
					{
						result = null;
					}
					else
					{
						result = result / factor;
					}
				}
			}
			// One factor is invalid
			else
			{
				result = null;
			}
		}
		
		return result;
	}
	
	/**
	 * Evaluates the value of the factor located in the
	 * given expression at the given position.
	 * 
	 * @param expr expression
	 * @param pos  position in the expression
	 * @return value of the factor
	 */
	private float evaluateFactor(String expr, IntPtr pos)
	{
		Float result;
		boolean negative = false;
		
		// Note whether factor is negative
		if (expr.charAt(pos.value) == '-')
		{
			negative = true;
			pos.increment();
		}
		
		// If a sub-expression found, evaluate recursively
		if (expr.charAt(pos.value) == '(')
		{
			pos.increment();
			result = evaluateExpression(expr, pos);
			
			// If no closing parens found, invalid
			if (expr.charAt(pos.value) != ')')
			{
				result = null;
			}
			
			pos.increment();
		}
		else
		{
			String token = "";
			char c;
			
			// Build factor token
			while (pos.value < expr.length())
			{
                
				c = expr.charAt(pos.value);
				if (Character.isDigit(c) || c == '.')
				{
					token += c;
					pos.increment();
				}
				else
					break;
			}
			
			// Convert token to float
			try
			{
				result = Float.valueOf(token);
			}
			catch (NumberFormatException nfe)
			{
				result = null;
			}
		}
		
		if (negative && result != null)
		{
			result = -1 * result;
		}
		
		return result;
	}
	
	/**
	 * Integer pointer. Since Java does not have pointers,
	 * this class serves as a "pointer" to an integer that
	 * can be modified.
	 * 
	 * @author Kyle Treubig <kyle@vimofthevine.com>
	 */
	private class IntPtr {
		
		private int value;
		
		IntPtr(int value)
		{
			this.value = value;
		}
		
		void increment()
		{
			value++;
		}
	}
	
}
