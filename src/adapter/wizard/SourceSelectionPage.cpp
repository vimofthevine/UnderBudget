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
#include "SourceSelectionPage.hpp"
#include "WizardPageId.hpp"

namespace ub {
namespace adapter {

//--------------------------------------------------------------------------------------------------
SourceSelectionPage::SourceSelectionPage(QWidget * parent)
        : QWizardPage(parent), gnucash_selection_(new QRadioButton(tr("GnuCash"))) {
    setTitle(tr("Select import source type"));

    gnucash_selection_->setChecked(true);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(gnucash_selection_);
    setLayout(layout);

    registerField("gnucash_selected", gnucash_selection_);
}

//--------------------------------------------------------------------------------------------------
int SourceSelectionPage::nextId() const {
    if (gnucash_selection_->isChecked()) {
        return static_cast<int>(WizardPageId::GnuCashPage);
    }
    return -1;
}

} // namespace adapter
} // namespace ub
