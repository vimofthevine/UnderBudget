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

#ifndef GENERALSETTINGS_HPP
#define GENERALSETTINGS_HPP

// Qt include(s)
#include <QGroupBox>

// Forward declaration(s)
class QCheckBox;

namespace ub {

/**
 * General settings page.
 *
 * @ingroup ui_prefs
 */
class GeneralSettings : public QGroupBox
{
	Q_OBJECT

public:
	/**
	 * Constructs a new general settings page.
	 *
	 * @param[in] parent parent widget
	 */
	GeneralSettings(QWidget* parent = 0);

public slots:
	/**
	 * Applies all changes to general settings.
	 */
	void apply();

	/**
	 * Restores all displayed settings to match the saved settings.
	 */
	void reset();

private:
	/** Auto re-import */
	QCheckBox* autoReImport;
};

}

#endif //GENERALSETTINGS_HPP
