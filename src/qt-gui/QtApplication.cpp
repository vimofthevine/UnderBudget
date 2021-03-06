/*
 * Copyright 2015 Kyle Treubig
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
#include <QtCore>

// UnderBudget include(s)
#include "info.hpp"
#include "QtApplication.hpp"
#include "ui/SdiWindow.hpp"

namespace ub {

//------------------------------------------------------------------------------
int QtApplication::start(int argc, char* argv[])
{
	QApplication app(argc, argv);
	setupApp(app);

	QString iconTheme = getenv("UB_ICON_THEME_OVERRIDE");
	if ( ! iconTheme.isEmpty())
	{
		QIcon::setThemeName(iconTheme);
		qDebug() << "Using icon theme" << QIcon::themeName();
	}

	SdiWindow* mainWindow = new SdiWindow();
	mainWindow->show();

	return app.exec();
}

}
