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

#ifndef PROGRESSDELEGATE_HPP
#define PROGRESSDELEGATE_HPP

// Qt include(s)
#include <QStyledItemDelegate>

namespace ub {

/**
 * Delegate to draw a progress bar for estimate progress.
 *
 * @ingroup ui_budget
 */
class ProgressDelegate : public QStyledItemDelegate
{
public:
	/**
	 * Constructs an estimate progress delegate.
	 *
	 * @param[in] parent parent object
	 */
	ProgressDelegate(QObject* parent);

	/**
	 * Reimplemented to paint a progress bar with a color appropriate
	 * for the health of an estimate's progress.
	 */
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};

}

#endif //PROGRESSDELEGATE_HPP
