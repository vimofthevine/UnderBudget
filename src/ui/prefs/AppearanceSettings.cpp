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
#include "ui/prefs/AppearanceSettings.hpp"

namespace ub {

//------------------------------------------------------------------------------
AppearanceSettings::AppearanceSettings(QWidget* parent)
	: QGroupBox(tr("Appearance Settings"), parent)
{
	// Create input fields
	useMdi = new QCheckBox(this);

	// Populate with initial values
	reset();

#ifndef Q_OS_WIN
	// Connect signals for immediate-apply
	connect(useMdi, SIGNAL(toggled(bool)), this, SLOT(apply()));
#endif

	// Put it all together...
	QFormLayout* form = new QFormLayout;
	form->addRow(tr("Use multiple document interface?"), useMdi);
	setLayout(form);
}

//------------------------------------------------------------------------------
void AppearanceSettings::apply()
{
	QSettings settings;
	settings.setValue(appearance::UseMDI, useMdi->isChecked());
}

//------------------------------------------------------------------------------
void AppearanceSettings::reset()
{
	QSettings settings;
	useMdi->setChecked(settings.value(appearance::UseMDI).toBool());
}

}

