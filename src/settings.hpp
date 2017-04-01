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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

namespace ub {

/**
 * Appearance settings keys.
 *
 * @ingroup main
 */
namespace appearance {

//------------------------------------------------------------------------------
const QString UseMDI = "UseMDI";

}

/**
 * Import settings keys.
 *
 * @ingroup main
 */
namespace import {

//------------------------------------------------------------------------------
const QString AutoReImport = "AutoReImportTransactions";

}

}

#endif //SETTINGS_HPP
