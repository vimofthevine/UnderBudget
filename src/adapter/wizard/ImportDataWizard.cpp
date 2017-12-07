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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <adapter/gnucash/GnuCashImporter.hpp>
#include <app/model/Repositories.hpp>
#include "GnuCashSourcePage.hpp"
#include "ImportDataWizard.hpp"
#include "SourceSelectionPage.hpp"
#include "WizardPageId.hpp"

namespace ub {
namespace adapter {

//--------------------------------------------------------------------------------------------------
ImportDataWizard::ImportDataWizard(std::shared_ptr<Repositories> repos, QWidget * parent)
        : QWizard(parent), repos_(repos) {
#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif
    setWindowTitle(tr("Import Financial Data"));

    setPage(static_cast<int>(WizardPageId::SourceSelectionPage), new SourceSelectionPage);
    setPage(static_cast<int>(WizardPageId::GnuCashPage), new GnuCashSourcePage);

    setStartId(static_cast<int>(WizardPageId::SourceSelectionPage));
}

//--------------------------------------------------------------------------------------------------
void ImportDataWizard::accept() {
    if (field("gnucash_selected").toBool()) {
        auto name = field("file_name").toString();
        QFileInfo info(name);
        if (info.exists()) {
            if (field("gnucash_sqlite").toBool()) {
                GnuCashImporter importer(repos_);
                importer.importFromSqlite(name, true);
            } else {
                qWarning() << "Importing GnuCash XML not yet supported";
            }
        }
    }

    QDialog::accept();
}

} // namespace adapter
} // namespace ub
