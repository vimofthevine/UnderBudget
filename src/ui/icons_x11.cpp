/*
 * Copyright 2014 Kyle Treubig
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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/icons.hpp"

namespace ub {

namespace Icon {

/**
 * Retrieves a theme icon of the given name. If no icon exists for the
 * given theme, then the appropriate icon from the Oxygen icon theme is
 * used.
 *
 * @param[in] name standard icon name
 * @return requested icon from the current theme
 */
QIcon get(const QString& name)
{
	QIcon icon = QIcon::fromTheme(name);
	if (icon.isNull())
	{
		icon.addFile(QString(":/icons/oxygen/22x22/%1").arg(name), QSize(22,22));
		icon.addFile(QString(":/icons/oxygen/48x48/%1").arg(name), QSize(48,48));
	}
	return icon;
}

//------------------------------------------------------------------------------
QIcon about()
{
	return get("help-about");
}

//------------------------------------------------------------------------------
QIcon analysisSummary()
{
	return get("view-statistics.png");
}

//------------------------------------------------------------------------------
QIcon assign()
{
	return get("go-next-use");
}

//------------------------------------------------------------------------------
QIcon calculate()
{
	return get("accessories-calculator");
}

//------------------------------------------------------------------------------
QIcon close()
{
	return get("document-close");
}

//------------------------------------------------------------------------------
QIcon closeAll()
{
	return QIcon();
}

//------------------------------------------------------------------------------
QIcon editBudget()
{
	return get("view-bank");
}

//------------------------------------------------------------------------------
QIcon editEstimates()
{
	return get("view-list-tree");
}

//------------------------------------------------------------------------------
QIcon editRules()
{
	return get("format-list-ordered");
}

//------------------------------------------------------------------------------
QIcon editPreferences()
{
	return get("preferences-other");
}

//------------------------------------------------------------------------------
QIcon estimateImpact()
{
	return get("view-financial-forecast");
}

//------------------------------------------------------------------------------
QIcon estimateProgress()
{
	return get("office-chart-bar-percentage");
}

//------------------------------------------------------------------------------
QIcon exit()
{
	return get("application-exit");
}

//------------------------------------------------------------------------------
QIcon exportFile()
{
	return get("document-export");
}

//------------------------------------------------------------------------------
QIcon importedTransactions()
{
	return get("wallet-open");
}

//------------------------------------------------------------------------------
QIcon importTransactions()
{
	return get("document-import");
}

//------------------------------------------------------------------------------
QIcon importTransactionsFrom()
{
	return QIcon();
}

//------------------------------------------------------------------------------
QIcon newDocument()
{
	return get("document-new");
}

//------------------------------------------------------------------------------
QIcon openDocument()
{
	return get("document-open");
}

//------------------------------------------------------------------------------
QIcon redo()
{
	return get("edit-redo");
}

//------------------------------------------------------------------------------
QIcon saveAs()
{
	return get("document-save-as");
}

//------------------------------------------------------------------------------
QIcon saveAsTemplate()
{
	return get("document-edit-sign");
}

//------------------------------------------------------------------------------
QIcon saveDocument()
{
	return get("document-save");
}

//------------------------------------------------------------------------------
QIcon undo()
{
	return get("edit-undo");
}

//------------------------------------------------------------------------------
QIcon windowCascade()
{
	return QIcon();
}

//------------------------------------------------------------------------------
QIcon windowTile()
{
	return QIcon();
}

}

}

