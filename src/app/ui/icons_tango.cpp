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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "icons.hpp"

namespace ub {
namespace Icon {

/**
 * Retrieves a theme icon of the given name. If no icon exists for the
 * given theme, then the appropriate icon from the built-in, tango-style
 * icon theme is used.
 *
 * @param[in] name standard icon name
 * @return requested icon from the current theme
 */
QIcon get(const QString & name) {
    QIcon icon = QIcon::fromTheme(name);
    if (icon.isNull()) {
        icon.addFile(QString(":/icons/tango/22x22/%1").arg(name), QSize(22, 22));
        icon.addFile(QString(":/icons/tango/48x48/%1").arg(name), QSize(48, 48));
    }
    return icon;
}

//------------------------------------------------------------------------------
QIcon about() {
    return get("help-about");
}

//------------------------------------------------------------------------------
QIcon analysisSummary() {
    return get("x-office-presentation");
}

//------------------------------------------------------------------------------
QIcon assign() {
    return get("assign-transactions");
}

//------------------------------------------------------------------------------
QIcon calculate() {
    return get("accessories-calculator");
}

//------------------------------------------------------------------------------
QIcon close() {
    return get("window-close");
}

//------------------------------------------------------------------------------
QIcon closeAll() {
    return QIcon();
}

//------------------------------------------------------------------------------
QIcon editBudget() {
    return get("edit-budget");
}

//------------------------------------------------------------------------------
QIcon editEstimates() {
    return get("edit-estimates");
}

//------------------------------------------------------------------------------
QIcon editRules() {
    return get("edit-rules");
}

//------------------------------------------------------------------------------
QIcon editPreferences() {
    return get("preferences-system");
}

//------------------------------------------------------------------------------
QIcon estimateImpact() {
    return get("view-financial-forecast");
}

//------------------------------------------------------------------------------
QIcon estimateProgress() {
    return get("x-office-spreadsheet");
}

//------------------------------------------------------------------------------
QIcon exit() {
    return get("application-exit");
}

//------------------------------------------------------------------------------
QIcon exportFile() {
    return get("document-export");
}

//------------------------------------------------------------------------------
QIcon importedTransactions() {
    return get("view-transactions");
}

//------------------------------------------------------------------------------
QIcon importTransactions() {
    return get("import-transactions");
}

//------------------------------------------------------------------------------
QIcon importTransactionsFrom() {
    return QIcon();
}

//------------------------------------------------------------------------------
QIcon newDocument() {
    return get("document-new");
}

//------------------------------------------------------------------------------
QIcon openDocument() {
    return get("document-open");
}

//------------------------------------------------------------------------------
QIcon redo() {
    return get("edit-redo");
}

//------------------------------------------------------------------------------
QIcon saveAs() {
    return get("document-save-as");
}

//------------------------------------------------------------------------------
QIcon saveAsTemplate() {
    return get("document-edit-sign");
}

//------------------------------------------------------------------------------
QIcon saveDocument() {
    return get("document-save");
}

//------------------------------------------------------------------------------
QIcon undo() {
    return get("edit-undo");
}

//------------------------------------------------------------------------------
QIcon windowCascade() {
    return QIcon();
}

//------------------------------------------------------------------------------
QIcon windowTile() {
    return QIcon();
}

} // Icon namespace
} // ub namespace
