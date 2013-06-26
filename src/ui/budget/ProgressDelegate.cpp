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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/budget/ProgressDelegate.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProgressDelegate::ProgressDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{ }

//------------------------------------------------------------------------------
void ProgressDelegate::paint(QPainter* painter,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// Decoded progress data
	// (first byte is health, remainder is percentage)
	const QByteArray progressData = index.data().toByteArray();
	bool healthy = progressData.left(1).toInt();
	int progress = progressData.right(progressData.length() - 1).toInt();
	progress = (progress > 100) ? 100 : progress;

	// Leave a little bit above/below progress bar
	QRect rect = option.rect;
	rect.setHeight(option.rect.height() - 2);
	rect.setTop(option.rect.top() + 1);

	QProgressBar progressBar;
	progressBar.resize(rect.size());
	progressBar.setMinimum(0);
	progressBar.setMaximum(100);
	progressBar.setValue(progress);
	progressBar.setTextVisible(false);

	QString stylesheet = QString(
		"QProgressBar { border:1px solid transparent; border-radius:3px; padding:0px;"
		"               background:transparent; }"
		"QProgressBar::chunk { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 white, stop:1 %1);"
		"               border-radius:3px; }")
		.arg(healthy ? "green" : "red");
	progressBar.setStyleSheet(stylesheet);

	// Not completely sure why this works, but this is what
	// QStyledItemDelegate::paint() does, except that when I do it here,
	// the text is not drawn (while it is if we call the parent paint())
	option.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);
	painter->save();
	painter->translate(rect.topLeft());
	progressBar.render(painter);
	painter->restore();
}

}

