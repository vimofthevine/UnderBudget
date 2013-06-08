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
#include "ui/widgets/LineEdit.hpp"

namespace ub {

//------------------------------------------------------------------------------
LineEdit::LineEdit(QWidget* parent)
	: QLineEdit(parent)
{
	setup();
}

//------------------------------------------------------------------------------
LineEdit::LineEdit(const QString& contents, QWidget* parent)
	: QLineEdit(contents, parent)
{
	setup();
}

//------------------------------------------------------------------------------
void LineEdit::setup()
{
	installEventFilter(new IgnoreUndoRedo(parent(), this));
}

}

