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
 * Creates an icon for an analysis summary action.
 *
 * @return analysis summary action icon
 */
QIcon analysisSummary();

/**
 * Creates an icon for an assign action.
 *
 * @return assign action icon
 */
QIcon assign();

/**
 * Creates an icon for a calculate action.
 *
 * @return calculate action icon
 */
QIcon calculate();

/**
 * Creates an icon for a close action.
 *
 * @return close action icon
 */
QIcon close();

/**
 * Creates an icon for a close-all action.
 *
 * @return close-all action icon
 */
QIcon closeAll();

/**
 * Creates an icon for an edit-budget action.
 *
 * @return edit-budget action icon
 */
QIcon editBudget();

/**
 * Creates an icon for an edit-estimates action.
 *
 * @return edit-estimates action icon
 */
QIcon editEstimates();

/**
 * Creates an icon for an edit-rules action.
 *
 * @return edit-rules action icon
 */
QIcon editRules();

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
 * Creates an icon for an estimate impact action.
 *
 * @return estimate impact action icon
 */
QIcon estimateImpact();

/**
 * Creates an icon for an estimate progress action.
 *
 * @return estimate progress action icon
 */
QIcon estimateProgress();

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
 * Creates an icon for an export file action.
 *
 * @return export file action icon
 */
QIcon exportFile();

/**
 * Creates an icon for an imported transactions action.
 *
 * @return imported transactions action icon
 */
QIcon importedTransactions();

/**
 * Creates an icon for an import-transactions action.
 *
 * @return import-transactions action icon
 */
QIcon importTransactions();

/**
 * Creates an icon for an import-transactions-from action.
 *
 * @return import-transactions-from action icon
 */
QIcon importTransactionsFrom();

/**
 * Creates an icon for a view-incomes action.
 *
 * @return view-incomes action icon
 */
QIcon incomes();

/**
 * Creates an icon for a new document action.
 *
 * @return new document action icon
 */
QIcon newDocument();

/**
 * Creates an icon for a open document action.
 *
 * @return open document action icon
 */
QIcon openDocument();

/**
 * Creates an icon for a redo action.
 *
 * @return redo action icon
 */
QIcon redo();

/**
 * Creates an icon for a view-reports action.
 *
 * @return view-reports action icon
 */
QIcon reports();

/**
 * Creates an icon for a save-as document action.
 *
 * @return save-as document action icon
 */
QIcon saveAs();

/**
 * Creates an icon for a save-as-template document action.
 *
 * @return save-as-template document action icon
 */
QIcon saveAsTemplate();

/**
 * Creates an icon for a save document action.
 *
 * @return save document action icon
 */
QIcon saveDocument();

/**
 * Creates an icon for an undo action.
 *
 * @return undo action icon
 */
QIcon undo();

/**
 * Creates an icon for a window cascade action.
 *
 * @return window cascade action icon
 */
QIcon windowCascade();

/**
 * Creates an icon for a window tile action.
 *
 * @return window tile action icon
 */
QIcon windowTile();

} // Icon namespace
} // ub namespace
