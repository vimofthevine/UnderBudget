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
#include <QWizardPage>

// Forward declaration(s)
class QLineEdit;
class QRadioButton;

namespace ub {
namespace adapter {

/**
 * Import-data wizard page with GnuCash source parameters.
 */
class GnuCashSourcePage : public QWizardPage {
    Q_OBJECT

public:
    /**
     * Initializes the wizard page.
     *
     * @param parent Parent widget
     */
    GnuCashSourcePage(QWidget * parent = 0);

    /**
     * Sets default values for all inputs.
     */
    void initializePage() override;

    /**
     * Verifies that all parameters are valid.
     *
     * @return @c true if parameters are valid
     */
    bool validatePage() override;

private slots:
    /**
     * Prompts the user to select a GnuCash file
     */
    void selectFile();

private:
    /** SQLite file type */
    QRadioButton * sqlite_type_;
    /** XML file type */
    QRadioButton * xml_type_;
    /** File selection */
    QLineEdit * file_selection_;
};

} // namespace adapter
} // namespace ub
