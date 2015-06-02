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

// CppMicroServices include(s)
#include <usSharedLibrary.h>

// UnderBudget include(s)
#include "UnderBudget/SettingsKeys.hpp"
#include "PluginLoader.hpp"

US_USE_NAMESPACE

namespace ub {

/**
 * Retrieves the configured plugin directory from settings, or determines the
 * location based on the location of the current application executable.
 *
 * @param[in] exe Location of the current application executable
 * @return Location of the plugin directory
 */
static QString pluginDirectory(const QString& exe)
{
	QSettings settings("vimofthevine", "UnderBudget");
	QString plugin_dir =
			settings.value(ub::plugins::PluginDirectory).toString();
	if ( ! plugin_dir.isEmpty())
	{
		return plugin_dir;
	}
	else
	{
		QFileInfo exe_info(exe);
		QDir dir = exe_info.absoluteDir();
		if (
				dir.cd("plugins") || // bin/plugins
				(dir.cdUp() && dir.cd("plugins")) || // bin/../plugins
				dir.cd("share/underbudget/plugins")) // bin/../share/...
		{
			settings.setValue(ub::plugins::PluginDirectory,
					dir.absolutePath());
			return dir.absolutePath();
		}
		else
		{
			qWarning() << "Plugins directory does not exist:"
				<< dir.absolutePath();
			return QString();
		}
	}
}

//------------------------------------------------------------------------------
void loadEnabledPlugins(const QString& exe)
{
	QString plugin_dir(pluginDirectory(exe));
	if (plugin_dir.isEmpty())
		return;
	qDebug() << "Plugin dir:" << plugin_dir;
	QDir dir(plugin_dir);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	QString so_suffix = QString::fromStdString(SharedLibrary().GetSuffix());
	dir.setNameFilters(QStringList(QString("*%1").arg(so_suffix)));
	auto plugins = dir.entryInfoList();
	for (auto plugin_info : plugins)
	{
		qDebug() << "Loading plugin:" << plugin_info.fileName();
		try
		{
			SharedLibrary so(plugin_info.absoluteFilePath().toStdString());
			so.Load();
		}
		catch (std::exception& e)
		{
			qWarning() << e.what();
		}
	}
}

}
