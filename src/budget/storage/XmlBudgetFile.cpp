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
#include "budget/storage/XmlBudgetFile.hpp"

namespace ub {

//------------------------------------------------------------------------------
XmlBudgetFile::XmlBudgetFile(const QString& fileName)
	: xmlFile(fileName)
{ }

//------------------------------------------------------------------------------
QSharedPointer<Budget> XmlBudgetFile::retrieve()
{
	QSharedPointer<Budget> budget;

	QFile file(xmlFile);
	if (file.open(QIODevice::ReadOnly))
	{
		if (reader.read(&file))
		{
			budget = reader.lastReadBudget();
			errorMsg = "";
		}
		else
		{
			errorMsg = reader.errorString();
		}
	}
	else
	{
		errorMsg = file.exists()
			? QObject::tr("File, %1, could not be read.\n%2").arg(xmlFile)
				.arg(file.errorString())
			: QObject::tr("File, %1, does not exist.").arg(xmlFile);
	}

	return budget;
}

//------------------------------------------------------------------------------
bool XmlBudgetFile::store(QSharedPointer<Budget> budget)
{
	return false;
}

//------------------------------------------------------------------------------
QString XmlBudgetFile::error() const
{
	return errorMsg;
}

//------------------------------------------------------------------------------
QString XmlBudgetFile::location() const
{
	return xmlFile;
}

}


