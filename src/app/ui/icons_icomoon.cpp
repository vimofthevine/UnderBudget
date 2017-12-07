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
    return QIcon(QString(":/icons/icomoon/%1.svg").arg(name));
}

//------------------------------------------------------------------------------
QIcon about() {
    return get("269-info");
}

//------------------------------------------------------------------------------
QIcon accounts() {
    return get("064-credit-card");
}

//------------------------------------------------------------------------------
QIcon add() {
    return get("267-plus");
}

//------------------------------------------------------------------------------
QIcon edit() {
    return get("006-pencil");
}

//------------------------------------------------------------------------------
QIcon editPreferences() {
    return QIcon();
}

//------------------------------------------------------------------------------
QIcon envelopes() {
    return get("070-envelop");
}

//------------------------------------------------------------------------------
QIcon exit() {
    return get("277-exit");
}

//------------------------------------------------------------------------------
QIcon expenses() {
    return get("059-cart");
}

//------------------------------------------------------------------------------
QIcon exportData() {
    return get("198-download2");
}

//------------------------------------------------------------------------------
QIcon importData() {
    return get("199-upload2");
}

//------------------------------------------------------------------------------
QIcon incomes() {
    return get("060-coin-dollar");
}

//------------------------------------------------------------------------------
QIcon openDocument() {
    return get("101-database");
}

//------------------------------------------------------------------------------
QIcon remove() {
    return get("173-bin");
}

//------------------------------------------------------------------------------
QIcon reports() {
    return get("156-stats-dots");
}

} // namespace Icon
} // namespace ub
