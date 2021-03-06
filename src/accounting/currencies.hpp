/*
 * Copyright 2013 Kyle Treubig
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

#ifndef CURRENCIES_HPP
#define CURRENCIES_HPP

// Qt include(s)
#include <QString>
#include <QStringList>

namespace ub {

/**
 * Retrieves a list of all supported currencies. The returned list
 * is a list of the supported currencies' ISO 4217 codes.
 *
 * @return supported currency ISO 4217 codes
 * @ingroup accounting
 */
const QStringList& supportedCurrencies();

/**
 * Retrieves the UTF symbol for the specified currency.
 *
 * @param[in] iso4217 ISO 4217 code for the desired currency
 * @return UTF symbol for the specified currency
 * @ingroup accounting
 */
QString currencySymbol(const QString& iso4217);

}

#endif //CURRENCIES_HPP
