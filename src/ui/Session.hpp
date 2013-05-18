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

protected:
	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the session.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);
};

}

#endif //SESSION_HPP
