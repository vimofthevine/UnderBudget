/*
 * Copyright 2015 Kyle Treubig
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

#pragma once

// Qt include(s)
#include <QString>

namespace ub {

/**
 * Loads all plugins discovered in the plugin directory that have not been
 * specifically disabled.
 *
 * If a plugin directory has not previously been configured as
 * @c ub::plugins::PluginDirectory, then the plugin directory will be
 * determined to be the @c plugins directory adjacent to the directory
 * containing the application executable (e.g., @c bin).
 *
 * @ingroup main
 * @param[in] exe Location of the current application executable
 */
void loadEnabledPlugins(const QString& exe);

}
