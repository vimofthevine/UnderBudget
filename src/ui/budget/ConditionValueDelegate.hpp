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

#ifndef CONDITIONVALUEDELEGATE_HPP
#define CONDITIONVALUEDELEGATE_HPP

// Qt include(s)
#include <QStyledItemDelegate>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

namespace ub {

/**
 * Custom delegate to allow editing of a condition value according to the current
 * field selection.
 */
class ConditionValueDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	/**
	 * Constructs a new condition value delegate.
	 *
	 * @param[in] parent parent object
	 */
	ConditionValueDelegate(QObject* parent = 0);

	/**
	 * Reimplemented to return an appropriate editor for the current
	 * field type.
	 */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the editor with the current value.
	 */
	void setEditorData(QWidget* editor, const QModelIndex& index) const;

	/**
	 * Reimplemented to apply the entered value to the condition.
	 */
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the editor's geometry.
	 */
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

private:
	/**
	 * Determines the field for the condition value located at the given index.
	 *
	 * @param[in] index condition value index
	 * @return condition field
	 */
	AssignmentRule::Field field(const QModelIndex& index) const;
};

}

#endif //CONDITIONVALUEDELEGATE_HPP
