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

#ifndef CONDITIONFIELDDELEGATE_HPP
#define CONDITIONFIELDDELEGATE_HPP

// Qt include(s)
#include <QStyledItemDelegate>

namespace ub {

/**
 * Custom delegate to allow editing of a condition field via a combo box.
 *
 * @ingroup ui_budget
 */
class ConditionFieldDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	/**
	 * Constructs a new condition field delegate.
	 *
	 * @param[in] parent parent object
	 */
	ConditionFieldDelegate(QObject* parent = 0);

	/**
	 * Reimplemented to return a QComboBox editor.
	 */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the QComboBox with the current field selection.
	 */
	void setEditorData(QWidget* editor, const QModelIndex& index) const;

	/**
	 * Reimplemented to apply the selected field to the condition.
	 */
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the QComboBox's geometry.
	 */
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};

}

#endif //CONDITIONFIELDDELEGATE_HPP
