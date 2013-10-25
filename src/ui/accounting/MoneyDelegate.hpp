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
#include <QStyledItemDelegate>

namespace ub {

/**
 * Custom delegate to allow editing of a money value via a money editor.
 */
class MoneyDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	/**
	 * Constructs a new money value delegate.
	 *
	 * @param[in] parent parent object
	 */
	MoneyDelegate(QObject* parent = 0);

	/**
	 * Reimplemented to return a MoneyEdit editor.
	 */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the MoneyEdit with the current money value.
	 */
	void setEditorData(QWidget* editor, const QModelIndex& index) const;

	/**
	 * Reimplemented to apply the new money value.
	 */
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const;

	/**
	 * Reimplemented to update the MoneyEdit's geometry.
	 */
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};

}

