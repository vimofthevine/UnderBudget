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

#ifndef PREFSDIALOG_HPP
#define PREFSDIALOG_HPP

// Qt include(s)
#include <QDialog>

// Forward declaration(s)
class QListWidget;
class QStackedWidget;

namespace ub {

/**
 * Application preferences dialog
 */
class PrefsDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * Constructs a new preferences dialog.
	 *
	 * @param[in] parent parent widget
	 */
	PrefsDialog(QWidget* parent = 0);

signals:
	/**
	 * Emitted when the user has requested for all changes to preferences
	 * be saved.
	 */
	void apply();

	/**
	 * Emitted when the user has cancelled, or rejected, any changes to
	 * preferences.
	 */
	void reset();

private:
	/** Settings page title selection list */
	QListWidget* list;
	/** Settings page widgets */
	QStackedWidget* stack;

	/**
	 * Adds the given settings page widget.
	 *
	 * @param[in] icon  icon resource name
	 * @param[in] title settings page title
	 * @param[in] page  settings page widget
	 */
	void add(const QString& icon, const QString& title, QWidget* page);
};

}

#endif //PREFSDIALOG_HPP
