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
#include "analysis/Actuals.hpp"
#include "analysis/Assignments.hpp"
#include "analysis/TransactionAssigner.hpp"
#include "budget/conditions.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"
#include "ledger/ImportedTransaction.hpp"

namespace ub {

//------------------------------------------------------------------------------
TransactionAssigner::TransactionAssigner(QSharedPointer<AssignmentRules> rules,
		Assignments* assignments, Actuals* actuals)
	: rules(rules), assignments(assignments),
	  actuals(actuals), isAssigning(false)
{ }

//------------------------------------------------------------------------------
void TransactionAssigner::assign(const QList<ImportedTransaction>& transactions)
{
	if ( ! isAssigning)
	{
		isAssigning = true;
		emit started();

		// Iterate over all transactions
		for (int i=0; i<transactions.size(); ++i)
		{
			assign(transactions.at(i));
		}

		isAssigning = false;
		emit finished();
	}
}

//------------------------------------------------------------------------------
void TransactionAssigner::assign(const ImportedTransaction& transaction)
{
	// Iterate over all rules
	for (int i=0; i<rules->size(); ++i)
	{
		const AssignmentRule* rule = rules->at(i);
		if (matches(transaction, rule))
		{
			actuals->record(rule->estimateId(), transaction.amount());
			assignments->record(transaction.transactionId(),
				rule->estimateId(), rule->ruleId());
			break;
		}
	}
}

//------------------------------------------------------------------------------
bool TransactionAssigner::matches(const ImportedTransaction& transaction,
	const AssignmentRule* rule)
{
	bool trnMatches(true);

	// Iterate over all conditions
	for (int k=0; k<rule->conditionCount(); ++k)
	{
		trnMatches &= matches(transaction, rule->conditionAt(k));
	}

	return trnMatches;
}

//------------------------------------------------------------------------------
bool TransactionAssigner::matches(const ImportedTransaction& transaction,
	const AssignmentRule::Condition& condition)
{
	switch (condition.field)
	{
	case AssignmentRule::Date:
		return qualifies(transaction.date(), condition);
	case AssignmentRule::Amount:
		return qualifies(transaction.amount(), condition);
	case AssignmentRule::Payee:
		return qualifies(transaction.payee(), condition);
	case AssignmentRule::Memo:
		return qualifies(transaction.memo(), condition);
	case AssignmentRule::DepositAccount:
		return qualifies(transaction.depositAccount(), condition);
	case AssignmentRule::WithdrawalAccount:
		return qualifies(transaction.withdrawalAccount(), condition);
	default:
		return false;
	}
}
}

