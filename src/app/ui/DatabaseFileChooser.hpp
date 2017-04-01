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
#include <QString>

namespace ub {

/**
 * Database file chooser utilities.
 */
class DatabaseFileChooser {
public:
    /**
     * Returns the location of the database file that was last opened.
     *
     * @return Location of the last database file that was opened, or an empty string
     *         if no such file exists.
     */
    static QString lastOpenedFile();

    /**
     * Prompts the user to select a database file to open.
     *
     * @param parent Parent widget
     * @return Location of the database file to be opened, or an empty string if
     *         the user did not select a file
     */
    static QString getFileToOpen(QWidget * parent);
};

} // ub namespace
