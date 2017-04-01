/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
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
    setWindowTitle(tr("Envelope"));
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
