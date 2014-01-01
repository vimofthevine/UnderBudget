/*
 * Copyright 2013 Kyle Treubig
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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "analysis/SortedDifferences.hpp"

namespace ub {

//------------------------------------------------------------------------------
SortedDifferences::SortedDifferences(QObject* parent)
	: QObject(parent)
{ }

//------------------------------------------------------------------------------
void SortedDifferences::clear()
{
	entries.clear();
	emit listChanged();
}

//------------------------------------------------------------------------------
void SortedDifferences::record(uint estimate, const Money& diff)
{
	DiffEntry entry;
	entry.estimate = estimate;
	entry.difference = diff;
	entries.append(entry);
	emit listChanged();
}

//------------------------------------------------------------------------------
void SortedDifferences::sort()
{
	qSort(entries);
	emit listChanged();
}

//------------------------------------------------------------------------------
int SortedDifferences::size() const
{
	return entries.size();
}

//------------------------------------------------------------------------------
uint SortedDifferences::estimate(int index) const
{
	if (index >= 0 && index < entries.size())
		return entries.at(index).estimate;
	return 0;
}

//------------------------------------------------------------------------------
Money SortedDifferences::difference(int index) const
{
	if (index >= 0 && index < entries.size())
		return entries.at(index).difference;
	return Money();
}

//------------------------------------------------------------------------------
bool operator<(const SortedDifferences::DiffEntry& lhs,
	const SortedDifferences::DiffEntry& rhs)
{
	// lhs is "less than" rhs when lhs.diff is greater than rhs.diff
	return (lhs.difference > rhs.difference);
}

}

