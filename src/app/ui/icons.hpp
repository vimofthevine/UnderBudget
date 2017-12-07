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

#pragma once

// Qt include(s)
#include <QIcon>

namespace ub {

/**
 * Functions for creating icon instances to be used throughout the
 * application.
 *
 * The implementations for these functions are platform-specific,
 * to provide native look-and-feel.
 *
 * @ingroup app
 */
namespace Icon {

/**
 * Creates an icon for an about action.
 *
 * @return about action icon
 */
QIcon about();

/**
 * Creates an icon for a view-accounts action.
 *
 * @return view-accounts action
 */
QIcon accounts();

/**
 * Creates an icon for an add action.
 *
 * @return add action icon
 */
QIcon add();

/**
 * Creates an icon for an edit action.
 *
 * @return edit action icon
 */
QIcon edit();

/**
 * Creates an icon for an edit-preferences action.
 *
 * @return edit-preferences action icon
 */
QIcon editPreferences();

/**
 * Creates an icon for a view-envelopes action.
 *
 * @return view-envelopes action icon
 */
QIcon envelopes();

/**
 * Creates an icon for an exit action.
 *
 * @return exit actioin icon
 */
QIcon exit();

/**
 * Creates an icon for a view-expenses action.
 *
 * @return view-expenses action icon
 */
QIcon expenses();

/**
 * Creates an icon for an export-data action.
 *
 * @return export-data action
 */
QIcon exportData();

/**
 * Creates an icon for an import-data action.
 *
 * @return import-data action icon
 */
QIcon importData();

/**
 * Creates an icon for a view-incomes action.
 *
 * @return view-incomes action icon
 */
QIcon incomes();

/**
 * Creates an icon for a open document action.
 *
 * @return open document action icon
 */
QIcon openDocument();

/**
 * Creates an icon for a delete/remove action.
 *
 * @return remove action icon
 */
QIcon remove();

/**
 * Creates an icon for a view-reports action.
 *
 * @return view-reports action icon
 */
QIcon reports();

} // namespace Icon
} // namespace ub
