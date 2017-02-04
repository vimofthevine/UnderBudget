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
#include <ledger/ui/AccountListWidget.hpp>
#include <ledger/ui/AccountModel.hpp>

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

    auto model = new ub::ledger::AccountModel;
    auto view = new ub::ledger::AccountListWidget(model, nullptr);

    if (repos and repos->isOpen()) {
        model->setRepositories(repos->accounts(), repos->transactions());
    }

    view->show();
	return app.exec();
}
