/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QIcon>
#include <QSettings>
#include <QDebug>

// UnderBudget include(s)
#include "info.hpp"
#include <app/model/DatabaseRepositories.hpp>
#include <app/model/Demo.hpp>
#include <app/ui/DatabaseFileChooser.hpp>
#include <app/ui/MainWindow.hpp>
#include <app/ui/MainWindowModel.hpp>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	setupApp(app);

    QCommandLineParser parser;
    parser.setApplicationDescription(app.tr("Personal finance and budget manager"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", app.tr("Database file to be opened"));

    QCommandLineOption demoOption("demo", app.tr("Open an in-memory demo database"));
    parser.addOption(demoOption);

    parser.process(app);

	QString iconTheme = getenv("UB_ICON_THEME_OVERRIDE");
    if (not iconTheme.isEmpty()) {
		QIcon::setThemeName(iconTheme);
	}
	qDebug() << "Using icon theme" << QIcon::themeName();

    std::shared_ptr<ub::DatabaseRepositories> repos;

    if (parser.isSet(demoOption)) {
        repos.reset(new ub::DatabaseRepositories(":memory:"));
        ub::Demo::populate(repos);
    } else {
        auto pos = parser.positionalArguments();
        if (pos.size() > 0) {
            repos.reset(new ub::DatabaseRepositories(pos[0]));
        }

        if (not repos or not repos->isOpen()) {
            auto file = ub::DatabaseFileChooser::lastOpenedFile();
            if (not file.isEmpty()) {
                repos.reset(new ub::DatabaseRepositories(file));
            }
        }

        if (not repos or not repos->isOpen()) {
            auto file = ub::DatabaseFileChooser::getFileToOpen(nullptr);
            if (not file.isEmpty()) {
                repos.reset(new ub::DatabaseRepositories(file));
            }
        }
    }

    auto window = new ub::MainWindow;
    auto model = new ub::MainWindowModel(window);

    if (repos and repos->isOpen()) {
        model->setRepositories(repos);
    }

    window->show();

	return app.exec();
}
