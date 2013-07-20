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
#include "ui/prefs/AppearanceSettings.hpp"
#include "ui/prefs/GeneralSettings.hpp"
#include "ui/prefs/PrefsDialog.hpp"

namespace ub {

//------------------------------------------------------------------------------
PrefsDialog::PrefsDialog(QWidget* parent)
	: QDialog(parent)
{
	// Set up setting category list and pages
	list = new QListWidget(this);
	list->setIconSize(QSize(32,32));
	stack = new QStackedWidget(this);
	connect(list, SIGNAL(currentRowChanged(int)),
		stack, SLOT(setCurrentIndex(int)));

	QHBoxLayout* pageLayout = new QHBoxLayout;
	pageLayout->addWidget(list);
	pageLayout->addWidget(stack, 1,
		Qt::AlignTop | Qt::AlignLeft);

	// Set up setting pages
	add(":/icons/general", tr("General"), new GeneralSettings(this));
	add(":/icons/appearance", tr("Appearance"), new AppearanceSettings(this));

	// Make sure something is selected initially
	list->setCurrentRow(0);

	// Set list's width so it only uses the bare minimum required size
	list->setFixedWidth(list->sizeHintForColumn(0)
		+ (2 * list->frameWidth()));

	// Add the setting pages to the dialog layout
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(pageLayout);

#ifndef Q_OS_MAC // OS X prefs don't have any buttons

	// Set up the buttons
	QDialogButtonBox* buttons = new QDialogButtonBox(
#ifdef Q_OS_WIN // Windows uses OK/cancel/apply
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox ::Apply,
#else // Unix
		QDialogButtonBox::Close,
#endif
		Qt::Horizontal, this);

	// Save changes to preferences
	connect(buttons, SIGNAL(accepted()), this, SIGNAL(apply()));
	connect(buttons->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
		this, SIGNAL(apply()));

	// Reset all inputs if changes are rejected
	connect(this, SIGNAL(rejected()), this, SIGNAL(reset()));

	// Close the dialog in response to buttons
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));

	mainLayout->addWidget(buttons);

#endif // Q_OS_MAC

	setLayout(mainLayout);
	setWindowTitle(tr("Preferences"));
}

//------------------------------------------------------------------------------
void PrefsDialog::add(const QString& icon, const QString& title, QWidget* page)
{
	connect(this, SIGNAL(apply()), page, SLOT(apply()));
	connect(this, SIGNAL(reset()), page, SLOT(reset()));

	QListWidgetItem* item = new QListWidgetItem(QIcon(icon), title, list);
	list->addItem(item);
	stack->addWidget(page);
}

}

