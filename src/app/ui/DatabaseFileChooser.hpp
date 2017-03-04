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
