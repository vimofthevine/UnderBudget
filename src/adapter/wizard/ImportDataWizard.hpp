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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QWizard>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>

namespace ub {
namespace adapter {

/**
 * Wizard for selecting import data sources.
 */
class ImportDataWizard : public QWizard {
    Q_OBJECT

public:
    /**
     * Initializes the wizard.
     *
     * @param repos  Application repositories
     * @param parent Parent widget
     */
    ImportDataWizard(std::shared_ptr<Repositories> repos, QWidget * parent = 0);

    /**
     * Performs the import with the selected parameters.
     */
    void accept() override;

private:
    /** Application repositories */
    std::shared_ptr<Repositories> repos_;
};

} // namespace adapter
} // namespace ub
