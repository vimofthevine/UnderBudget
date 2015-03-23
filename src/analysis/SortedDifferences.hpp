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

#ifndef SORTEDDIFFERENCES_HPP
#define SORTEDDIFFERENCES_HPP

// Qt include(s)
#include <QList>
#include <QObject>

// UnderBudget include(s)
#include "accounting/Money.hpp"

namespace ub {

/**
 * A list of estimated vs actual differences, sorted in descending
 * order.
 *
 * @ingroup analysis
 */
class SortedDifferences : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a sorted differences list.
	 *
	 * @param[in] parent parent object
	 */
	SortedDifferences(QObject* parent = 0);

	/**
	 * Clears all recorded differences.
	 */
	void clear();

	/**
	 * Records an estimated vs actual difference for the given
	 * estimate. If this method is called twice for the same
	 * estimate, both entries will be added to the list. Thus,
	 * be sure to use `clear()` between uses.
	 *
	 * @param[in] estimate unique ID for which to record a difference
	 * @param[in] diff     estimated vs actual difference for the estimate
	 */
	void record(uint estimate, const Money& diff);

	/**
	 * Sorts the list in a descending order, such that the first
	 * item in the list represents the biggest difference.
	 */
	void sort();

	/**
	 * Returns the size of the list.
	 *
	 * @return list size
	 */
	int size() const;

	/**
	 * Returns the unique ID of the estimate at the given index.
	 * If the index is not valid, a value of 0 is returned.
	 *
	 * @param[in] index list index
	 * @return estimate unique ID at the given index
	 */
	uint estimate(int index) const;

	/**
	 * Returns the difference amount at the given index. If the
	 * index is not valid, a money value of 0 is returned.
	 *
	 * @param[in] index list index
	 * @return difference amount at the given index
	 */
	Money difference(int index) const;

signals:
	/**
	 * Emitted whenever the list changes. The nature of the change
	 * may be a clearing of the list, a new entry, or a sorting of
	 * the list.
	 */
	void listChanged();

private:
	/**
	 * Difference entry struct
	 */
	struct DiffEntry {
		/** Estimate unique ID */
		uint estimate;
		/** Difference amount */
		Money difference;
	};

	/** List of difference entries */
	QList<DiffEntry> entries;

	/**
	 * Checks if the first entry belongs "before" the second entry.
	 * An entry is considered "before" another when its difference
	 * value is greater than the other.
	 *
	 * @param[in] lhs first entry
	 * @param[in] rhs second entry
	 * @return `true` if the first entry's difference value is greater
	 *         than the second entry's
	 */
	friend bool operator<(const DiffEntry& lhs, const DiffEntry& rhs);
};

}

#endif //SORTEDDIFFERENCES_HPP
