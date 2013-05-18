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

#ifndef SESSION_HPP
#define SESSION_HPP

// Qt include(s)
#include <QStackedWidget>

namespace ub {

/**
 * Widget for an open budget session.
 */
class Session : public QStackedWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new
	 */
	Session(QWidget* parent = 0);

	/**
	 * Creates a new budget file
	 */
	void newBudgetFile();

	/**
	 * Opens the budget defined in the specified file.
	 *
	 * @param[in] file budget file name
	 * @return `true` if the file was successfully opened
	 */
	bool openBudgetFile(const QString& file);

	/**
	 * Saves any modifications to the budget.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool save();

	/**
	 * Saves this budget to a new file.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool saveAs();

	/**
	 * Saves this budget as the template budget.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool saveAsTemplate();

	/**
	 * Returns the name of the budget associated with this session.
	 *
	 * @return name of the current budget
	 */
	QString budgetName() const;

	/**
	 * Returns the name of the file in which the current budget
	 * is defined.
	 *
	 * @return name of the current budget file
	 */
	QString currentFileName() const;

signals:
	/**
	 * Emitted when the name of the budget belonging to this session changes.
	 *
	 * @param name new budget name
	 */
	void budgetNameChanged(QString name);

protected:
	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the session.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);

private:
	/** Name of the current budget file */
	QString currentFile;
	/** Whether the current budget file is untitled (e.g., unsaved) */
	bool isUntitled;
};

}

#endif //SESSION_HPP
