/*
 * Copyright 2017 Kyle Treubig
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
#include <ledger/model/Envelope.hpp>
#include <ledger/ui/EnvelopeModel.hpp>
#include "EnvelopeDetailsDialog.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeDetailsDialog::EnvelopeDetailsDialog(EnvelopeModel * model, QWidget * parent)
        : QDialog(parent), model_(model) {
    name_ = new QLineEdit(this);
    name_->setPlaceholderText(tr("Envelope name"));

    buttons_ = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset |
                                    QDialogButtonBox::Cancel);
    connect(buttons_, &QDialogButtonBox::clicked, this, &EnvelopeDetailsDialog::clicked);

    QFormLayout * form = new QFormLayout;
    form->addRow(tr("Name"), name_);
    form->addRow("", buttons_);

    setLayout(form);
}

//--------------------------------------------------------------------------------------------------
void EnvelopeDetailsDialog::resetForNewEnvelope(const QModelIndex & parent) {
    parent_index_ = parent;
    envelope_index_ = QModelIndex();
    envelope_ = Envelope();
    name_->clear();
    show();
}

//--------------------------------------------------------------------------------------------------
void EnvelopeDetailsDialog::showEnvelope(const QModelIndex & index) {
    parent_index_ = model_->parent(index);
    envelope_index_ = index;
    envelope_ = model_->envelope(index);
    name_->setText(envelope_.name());
    show();
}

//--------------------------------------------------------------------------------------------------
void EnvelopeDetailsDialog::clicked(QAbstractButton * button) {
    if (button == buttons_->button(QDialogButtonBox::Save)) {
        Envelope envelope = envelope_;
        envelope.setName(name_->text());
        if (not envelope_index_.isValid()) {
            if (model_->create(envelope, parent_index_)) {
                emit accept();
            }
        } else {
            if (model_->update(envelope, envelope_index_)) {
                emit accept();
            }
        }
    } else if (button == buttons_->button(QDialogButtonBox::Reset)) {
        name_->setText(envelope_.name());
    } else if (button == buttons_->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

} // ledger namespace
} // ub namespace
