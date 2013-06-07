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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/widgets/IgnoreUndoRedo.hpp"

namespace ub {

//------------------------------------------------------------------------------
IgnoreUndoRedo::IgnoreUndoRedo(QObject* recipient, QObject* parent)
	: QObject(parent), recipient(recipient)
{ }

//------------------------------------------------------------------------------
bool IgnoreUndoRedo::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->matches(QKeySequence::Undo)
			|| keyEvent->matches(QKeySequence::Redo))
		{
			QCoreApplication::postEvent(recipient, new QKeyEvent(
				keyEvent->type(), keyEvent->key(), keyEvent->modifiers(),
				keyEvent->nativeScanCode(), keyEvent->nativeVirtualKey(),
				keyEvent->nativeModifiers(), keyEvent->text(),
				keyEvent->isAutoRepeat(), keyEvent->count()
			));
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return QObject::eventFilter(watched, event);
}

}

