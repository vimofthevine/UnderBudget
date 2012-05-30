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

package com.vimofthevine.underbudget.core.report;

/**
 * Enumeration of rationale for the calculation
 * of the expected amount of estimates
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum ExpectedAmountRationale {
	BUDGET_IS_OVER,
	NO_MORE_TRANSACTIONS,
	ACTUAL_EXCEEDS_ESTIMATED,
	ESTIMATED_EXCEEDS_ACTUAL;
}
