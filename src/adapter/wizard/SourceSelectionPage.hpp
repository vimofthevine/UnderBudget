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
class QRadioButton;
class QWidget;

namespace ub {
namespace adapter {

/**
 * Import-data wizard page with source selection parameters.
 */
class SourceSelectionPage : public QWizardPage {
    Q_OBJECT

public:
    /**
     * Initializes the wizard page.
     *
     * @param parent Parent widget
     */
    SourceSelectionPage(QWidget * parent = 0);

    /**
     * Returns the ID of the next page to be presented, based on the source selection.
     *
     * @return ID of the next page
     */
    int nextId() const override;

private:
    /** GnuCash selection */
    QRadioButton * gnucash_selection_;
};

}
}
