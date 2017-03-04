#pragma once

// Qt include(s)
#include <QDialog>
#include <QModelIndex>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>

// Forward declaration(s)
class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class EnvelopeModel;

/**
 * User input widget for creating and modifying envelopes.
 *
 * @ingroup ledger
 */
class EnvelopeDetailsDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Initializes the envelope details widget.
     *
     * @param model  Envelope model
     * @param parent Parent widget
     */
    EnvelopeDetailsDialog(EnvelopeModel * model, QWidget * parent);

public slots:
    /**
     * Updates the form to define a new envelope.
     *
     * @param[in] parent Model index of the parent envelope under which to create a new envelope
     */
    void resetForNewEnvelope(const QModelIndex & parent);

    /**
     * Updates the form to modify an existing envelope.
     *
     * @param[in] index Model index of the envelope to be modified
     */
    void showEnvelope(const QModelIndex & index);

protected slots:
    /**
     * Responds to a button click as appropriate.
     */
    void clicked(QAbstractButton * button);

private:
    /** Envelope model */
    EnvelopeModel * model_;
    /** Envelope name input widget */
    QLineEdit * name_;
    /** Button box */
    QDialogButtonBox * buttons_;

    /** Parent model index */
    QModelIndex parent_index_;
    /** Envelope model index */
    QModelIndex envelope_index_;
    /** Envelope shown in the form */
    Envelope envelope_;
};

} // ledger namespace
} // ub namespace
