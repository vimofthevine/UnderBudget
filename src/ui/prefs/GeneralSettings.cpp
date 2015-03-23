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
#include "settings.hpp"
#include "ui/prefs/GeneralSettings.hpp"

namespace ub {

//------------------------------------------------------------------------------
GeneralSettings::GeneralSettings(QWidget* parent)
	: QGroupBox(tr("General Settings"), parent)
{
	// Create input fields
	autoReImport = new QCheckBox(this);

	// Populate with initial values
	reset();

#ifndef Q_OS_WIN
	// Connect signals for immediate-apply
	connect(autoReImport, SIGNAL(toggled(bool)), this, SLOT(apply()));
#endif

	// Import settings
	QGroupBox* importSettings = new QGroupBox(tr("Transaction Import Settings"));
	{
		QFormLayout* form = new QFormLayout;
		form->addRow(tr("Automatically re-import transactions when\n"
			"external file change detected? (requires restart)"), autoReImport);
		importSettings->setLayout(form);
	}

	// Put it all together...
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(importSettings);
	setLayout(layout);
}

//------------------------------------------------------------------------------
void GeneralSettings::apply()
{
	QSettings settings;
	settings.setValue(import::AutoReImport, autoReImport->isChecked());
}

//------------------------------------------------------------------------------
void GeneralSettings::reset()
{
	QSettings settings;
	autoReImport->setChecked(settings.value(import::AutoReImport).toBool());
}

}


