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

#ifndef IGNOREUNDOREDO_HPP
#define IGNOREUNDOREDO_HPP

// Qt include(s)
#include <QObject>

namespace ub {

class IgnoreUndoRedo : public QObject
{
public:
	/**
	 * Constructs an ignore-undo/redo event filter.
	 *
	 * @param[in] recipient undo/redo event recipient
	 * @param[in] parent parent object
	 */
	IgnoreUndoRedo(QObject* recipient, QObject* parent = 0);

	/**
	 * Reimplemented event filter to remove/ignore the undo/redo
	 * actions on a widget so that the application undo/redo
	 * actions are executed.
	 */
	bool eventFilter(QObject* watched, QEvent* event);

private:
	/** Object to receive undo/redo events */
	QObject* recipient;
};

}

#endif //IGNOREUNDOREDO_HPP
