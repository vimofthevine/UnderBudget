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
#include "GnuCashSourcePage.hpp"
#include "WizardPageId.hpp"

namespace ub {
namespace adapter {

//--------------------------------------------------------------------------------------------------
const QString LAST_IMPORTED_GNUCASH_FILE = "LastImportedGnuCashFile";
const QString LAST_IMPORTED_GNUCASH_TYPE = "LastImportedGnuCashType";
const std::string GNUCASH_FILE_FILTER = "GnuCash (*.gnucash);;All (*)";

//--------------------------------------------------------------------------------------------------
GnuCashSourcePage::GnuCashSourcePage(QWidget * parent)
        : QWizardPage(parent), sqlite_type_(new QRadioButton(tr("SQLite file"))),
          xml_type_(new QRadioButton(tr("XML file"))), file_selection_(new QLineEdit) {
    setTitle(tr("Import GnuCash Database"));

    sqlite_type_->setChecked(true);
    xml_type_->setEnabled(false);

    auto browse = new QPushButton(tr("Browse"));
    connect(browse, &QPushButton::clicked, this, &GnuCashSourcePage::selectFile);

    auto file_layout = new QHBoxLayout;
    file_layout->addWidget(file_selection_);
    file_layout->addWidget(browse);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("File Type:")));
    layout->addWidget(sqlite_type_);
    layout->addWidget(xml_type_);
    layout->addLayout(file_layout);
    setLayout(layout);

    registerField("gnucash_sqlite", sqlite_type_);
    registerField("gnucash_xml", xml_type_);
    registerField("file_name*", file_selection_);

    setFinalPage(true);
}

//--------------------------------------------------------------------------------------------------
void GnuCashSourcePage::initializePage() {
    QSettings settings;
    auto last = settings.value(LAST_IMPORTED_GNUCASH_FILE).toString();
    QFileInfo info(last);
    if (info.exists()) {
        file_selection_->setText(last);
    }
}

//--------------------------------------------------------------------------------------------------
bool GnuCashSourcePage::validatePage() {
    auto name = file_selection_->text();
    if (not name.isEmpty()) {
        QFileInfo info(name);
        if (info.exists()) {
            QSettings settings;
            settings.setValue(LAST_IMPORTED_GNUCASH_FILE, info.canonicalFilePath());
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
void GnuCashSourcePage::selectFile() {
    auto name = QFileDialog::getOpenFileName(parentWidget(), tr("Select GnuCash file"), QString(),
                                             tr(GNUCASH_FILE_FILTER.c_str()));
    if (not name.isEmpty()) {
        QFileInfo info(name);
        if (info.exists()) {
            file_selection_->setText(name);
        }
    }
}

} // namespace adapter
} // namespace ub
