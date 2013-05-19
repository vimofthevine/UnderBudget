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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "budget/storage/SqlBudgetFile.hpp"

namespace ub {

//------------------------------------------------------------------------------
SqlBudgetFile::SqlBudgetFile(const QString& fileName)
	: sqliteFile(fileName)
{ }

//------------------------------------------------------------------------------
QSharedPointer<Budget> SqlBudgetFile::retrieve()
{
	QSharedPointer<Budget> budget;
	return budget;
}

//------------------------------------------------------------------------------
bool SqlBudgetFile::store(QSharedPointer<Budget> budget)
{
	return false;
}

//------------------------------------------------------------------------------
QString SqlBudgetFile::error() const
{
	return errorMsg;
}

//------------------------------------------------------------------------------
QString SqlBudgetFile::location() const
{
	return sqliteFile;
}

}

