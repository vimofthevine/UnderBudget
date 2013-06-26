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

#ifndef ASSIGNMENTRULES_HPP
#define ASSIGNMENTRULES_HPP

// Qt include(s)
#include <QHash>
#include <QMultiHash>
#include <QObject>

namespace ub {

// Forward declaration(s)
class AssignmentRule;

/**
 * A user-defined list of assignment rules.
 */
class AssignmentRules : public QObject
{
	Q_OBJECT

public:
	/**
	 * Creates a new assignment rules list. The new list is allocated on the
	 * heap, and the memory is owned by the returned shared pointer. When all
	 * references to the list have gone out of scope, the `QSharedPointer`
	 * will delete the list, and all rules along with it.
	 *
	 * @return shared pointer to the assignment rules list
	 */
	static QSharedPointer<AssignmentRules> create();

	/**
	 * Adds a new rule to the end of this list. This should only be used
	 * when constructing the rules list from a persistent storage medium.
	 * Once constructed, new rules should be added to the list using
	 * `addRule()`.
	 *
	 * @param[in] ruleId     unique ID
	 * @param[in] estimateId associated estimate ID
	 * @param[in] conditions rule conditions
	 */
	AssignmentRule* createRule(uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions);

	/**
	 * Searches the rules list for a rule with the given ID.
	 *
	 * @param[in] ruleId ID of the rule to be searched
	 * @return assignment rule, if found, else a null pointer
	 */
	AssignmentRule* find(uint ruleId) const;

private:
	/** List of rules */
	QList<AssignmentRule*> rules;
	/** Map of rule IDs to indices (for faster lookup) */
	QHash<uint, int> ridToIndex;
	/** Map of estimate IDs to indices (for estimate-based lookup) */
	QMultiHash<uint, int> eidToIndex;

	/**
	 * Constructs a new assignment rules list. This constructor is
	 * private so that client code cannot create an assignment
	 * rule list on the stack.
	 */
	AssignmentRules();
};

}

#endif //ASSIGNMENTRULES_HPP
