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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

// Qt include(s)
#include <QMainWindow>

namespace ub
{

/**
 * The main, primary application window.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * Initializes the main window.
	 */
	MainWindow();

protected:
	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the application.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);

private slots:
	/**
	 * Displays information about the application.
	 */
	void about();

private:
	/**
	 * Store window settings such as size, geometry, etc.
	 */
	void writeSettings();

	/**
	 * Restore saved window settings.
	 */
	void readSettings();
};

}

#endif //MAINWINDOW_HPP
