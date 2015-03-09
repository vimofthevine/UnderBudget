/*
 * Copyright 2014 Kyle Treubig
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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "budget/UIPrefs.hpp"

namespace ub {

//------------------------------------------------------------------------------
QSharedPointer<UIPrefs> UIPrefs::create()
{
	return QSharedPointer<UIPrefs>(new UIPrefs());
}

//------------------------------------------------------------------------------
QStringList UIPrefs::allKeys() const
{
	return preferences.keys();
}

//------------------------------------------------------------------------------
void UIPrefs::clear()
{
	preferences.clear();
}

//------------------------------------------------------------------------------
void UIPrefs::remove(const QString& key)
{
	preferences.remove(key);
}

//------------------------------------------------------------------------------
void UIPrefs::setValue(const QString& key, const QVariant& val)
{
	preferences.insert(key, val);
}

//------------------------------------------------------------------------------
QVariant UIPrefs::value(const QString& key, const QVariant& def) const
{
	return preferences.value(key, def);
}

}
