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

namespace ub {

/**
 * Starting point for the application.
 *
 * This should be invoked by @c main.
 */
class Application
{
public:
	/**
	 * Start the application with the given program arguments.
	 *
	 * @param[in] argc Number of arguments
	 * @param[in] argv Array of arguments
	 * @return Application exit status
	 */
	virtual int start(int argc, char* argv[]) = 0;
};

}
