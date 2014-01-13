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

#ifndef ICONS_HPP
#define ICONS_HPP

// Qt include(s)
#include <QIcon>

// Forward declaration(s)
class QWidget;

namespace ub {

/**
 * Functions for creating icon instances to be used throughout the
 * application.
 *
 * The implementations for these functions are platform-specific,
 * to provide native look-and-feel.
 */
namespace Icon {

/**
 * Creates an icon for an about action.
 *
 * @return about action icon
 */
QIcon about();

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

}

}

#endif //ICONS_HPP
