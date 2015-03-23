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

#ifndef UIPREFS_HPP
#define UIPREFS_HPP

// Qt include(s)
#include <QSharedPointer>

namespace ub {

/**
 * Budget-specific UI preferences.
 *
 * @ingroup budget
 */
class UIPrefs {

public:
	/**
	 * Creates a new UI preferences instance. The new instance is allocated
	 * on the heap, and the memory is owned by the returned shared pointer.
	 *
	 * @return shared pointer to the UI preferences
	 */
	static QSharedPointer<UIPrefs> create();

	/**
	 * Returns a list of all keys for stored UI preferences.
	 *
	 * @return all UI preference keys
	 */
	QStringList allKeys() const;

	/**
	 * Removes all UI preference entries.
	 */
	void clear();

	/**
	 * Removes the UI preference with the specified key.
	 *
	 * @param[in] key UI preference key
	 */
	void remove(const QString& key);

	/**
	 * Sets the UI preference value for the specified key.
	 *
	 * @param[in] key UI preference key
	 * @param[in] val UI preference value
	 */
	void setValue(const QString& key, const QVariant& val);

	/**
	 * Returns the UI preference value for the specified key, or
	 * a default value if none exists.
	 *
	 * @param[in] key UI preference key
	 * @param[in] def default value
	 * @return UI preference value for the specified key, or the
	 *         default value if none exists
	 */
	QVariant value(const QString& key, const QVariant& def = QVariant()) const;

private:
	/** Map of UI preferences */
	QMap<QString,QVariant> preferences;
};

}

#endif //UIPREFS_HPP
