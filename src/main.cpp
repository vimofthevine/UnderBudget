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

// CppMicroServices include(s)
#include <usGetModuleContext.h>
#include <usModuleContext.h>

// UnderBudget include(s)
#include <UnderBudget/Application.hpp>
#include "PluginLoader.hpp"

US_USE_NAMESPACE

int main(int argc, char* argv[])
{
	ub::loadEnabledPlugins(argv[0]);
	ModuleContext* context = GetModuleContext();
	auto ref = context->GetServiceReference<ub::Application>();
	if ( ! ref)
	{
		std::cerr << "No application service found, exiting." << std::endl;
		return 1;
	}
	else
	{
		auto app = context->GetService(ref);
		return app->start(argc, argv);
	}
}
