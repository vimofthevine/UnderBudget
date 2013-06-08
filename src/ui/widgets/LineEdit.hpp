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

#ifndef LINEEDIT_HPP
#define LINEEDIT_HPP

// Qt include(s)
#include <QLineEdit>

// Forward declaration(s)
class QContextMenuEvent;

namespace ub {

/**
 * Customized QLineEdit that removes the built-in undo/redo functionality.
 */
class LineEdit : public QLineEdit
{
	Q_OBJECT

public:
	/**
	 * Constructs a line edit with no text.
	 *
	 * @param[in] parent parent widget
	 */
	LineEdit(QWidget* parent = 0);

	/**
	 * Constructs a line edit with the text contents.
	 *
	 * @param[in] contents line edit text contents
	 * @param[in] parent   parent widget
	 */
	LineEdit(const QString& contents, QWidget* parent = 0);

	/**
	 * Reimplemented to omit the undo/redo actions.
	 */
	QMenu* createStandardContextMenu();

public slots:
	/**
	 * Reimplemented to only set the text if it differs from the
	 * current editor contents. This eliminates an annoying behavior
	 * where the cursor is moved to the end of the editor while
	 * typing (due to the constant setting of the editor contents
	 * due to the use of the command pattern).
	 */
	void setText(const QString& contents);

protected:
	/**
	 * Removes undo/redo actions from the context menu.
	 */
	void contextMenuEvent(QContextMenuEvent* event);

private:
	/** Enables or disables the built-in undo/redo functionality */
	bool undoRedoEnabled;

	/**
	 * Sets up the customized text editor.
	 */
	void setup();
};

}

#endif //LINEEDIT_HPP
