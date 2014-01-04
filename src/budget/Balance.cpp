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
#include "budget/AddContributorCommand.hpp"
#include "budget/Balance.hpp"
#include "budget/RemoveContributorCommand.hpp"
#include "budget/UpdateContributorCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
QSharedPointer<Balance> Balance::create(const Money& value)
{
	return QSharedPointer<Balance>(new Balance(value));
}

//------------------------------------------------------------------------------
Balance::Balance(const Money& value)
{
	contributors << Contributor(tr("Balance"), value, true);
}

//------------------------------------------------------------------------------
QSharedPointer<Balance> Balance::create(const QList<Contributor>& contributors)
{
	return QSharedPointer<Balance>(new Balance(contributors));
}

//------------------------------------------------------------------------------
Balance::Balance(const QList<Contributor>& contributors)
	: contributors(contributors)
{ }

//------------------------------------------------------------------------------
QUndoCommand* Balance::addContributor(QUndoCommand* cmd)
{
	return new AddContributorCommand(this, cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* Balance::removeContributor(int index, QUndoCommand* cmd)
{
	if (contributors.size() == 0)
		return new QUndoCommand(cmd);
	return new RemoveContributorCommand(this, index, cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* Balance::updateContributor(int index, const Contributor& contributor,
	QUndoCommand* cmd)
{
	return new UpdateContributorCommand(this, index, contributor, cmd);
}

//------------------------------------------------------------------------------
int Balance::contributorCount() const
{
	return contributors.size();
}

//------------------------------------------------------------------------------
Balance::Contributor Balance::contributorAt(int index) const
{
	return contributors.value(index, Contributor());
}

//------------------------------------------------------------------------------
Money Balance::value() const
{
	Money amount;

	for (int i=0; i<contributors.size(); ++i)
	{
		Contributor contributor = contributors.at(i);
		if (contributor.increase)
			amount += contributor.amount;
		else
			amount -= contributor.amount;
	}

	return amount;
}

//------------------------------------------------------------------------------
void Balance::addContributor(const Contributor& contributor, int index)
{
	if (index < 0)
		index = 0;
	if (index > contributors.size())
		index = contributors.size();
	contributors.insert(index, contributor);
	emit contributorAdded(contributor, index);
	emit valueChanged();
}

//------------------------------------------------------------------------------
void Balance::deleteContributor(int index)
{
	if (contributors.size() == 0)
		return;

	if (index < 0)
		index = 0;
	if (index >= contributors.size())
		index = contributors.size() - 1;
	Contributor contributor = contributors.takeAt(index);
	emit contributorRemoved(contributor, index);
	emit valueChanged();
}

//------------------------------------------------------------------------------
void Balance::updateContributor(const Contributor& contributor, int index)
{
	if (index < 0)
		index = 0;
	if (index >= contributors.size())
		index = contributors.size() - 1;
	contributors.replace(index, contributor);
	emit contributorUpdated(contributor, index);
	emit valueChanged();
}

}

