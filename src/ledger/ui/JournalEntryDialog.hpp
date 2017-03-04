#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QDialog>

// UnderBudget include(s)
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/Transaction.hpp>

// Forward declaration(s)
class QCheckBox;
class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QTableView;
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountSplitModel;
class DoubleLineEdit;
class EnvelopeSplitModel;
class LedgerRepository;

/**
 * User input widget for creating and modifying journal entries.
 */
class JournalEntryDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Initializes the journal entry dialog.
     *
     * @param parent Parent widget
     */
    JournalEntryDialog(QWidget * parent);

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<LedgerRepository> repository);

signals:
    /**
     * Emitted when an error has occurred.
     *
     * @param message Error message
     */
    void error(const QString & message) const;

public slots:
    /**
     * Clears and displays the dialog to allow defining a new journal entry.
     */
    void prepareForNewEntry();

    /**
     * Updates and displays the dialog to allow modifying an existing journal entry.
     *
     * @param transaction Transaction to be modified
     */
    void prepareForModification(const Transaction & transaction);

    /**
     * Updates and displays the dialog to allow duplicating an existing journal entry.
     *
     * @param transaction Transaction to be duplicated
     */
    void prepareForDuplication(const Transaction & transaction);

    /**
     * Prompts the user to confirm deletion of the requested transaction.
     *
     * @param transaction Transaction to be deleted
     */
    void deleteTransaction(const Transaction & transaction);

private slots:
    /**
     * Updates the input widgets with the selected account split for modification.
     * @param current  Model index of the currently selected account split
     * @param previous Model index of the previously selected account split
     */
    void selectAccountSplit(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Saves the account split from the input form into the journal entry.
     */
    void saveAccountSplit();

    /**
     * Clears the account split input form.
     */
    void clearAccountSplit();

    /**
     * Deletes the selected account split from the journal entry.
     */
    void deleteAccountSplit();

    /**
     * Updates the input widgets with the selected envelope split for modification.
     * @param current  Model index of the currently selected envelope split
     * @param previous Model index of the previously selected envelope split
     */
    void selectEnvelopeSplit(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Saves the envelope split from the input form into the journal entry.
     */
    void saveEnvelopeSplit();

    /**
     * Clears the envelope split input form.
     */
    void clearEnvelopeSplit();

    /**
     * Deletes the selected envelope split from the journal entry.
     */
    void deleteEnvelopeSplit();

    /**
     * Responds to a button click as appropriate.
     */
    void clicked(QAbstractButton * button);

private:
    /** Ledger repository */
    std::shared_ptr<LedgerRepository> repository_;

    /** Date input widget */
    QDateEdit * date_;
    /** Payee input widget */
    QLineEdit * payee_;

    /** Account selection widget */
    QComboBox * account_;
    /** Account transaction amount input widget */
    DoubleLineEdit * account_amount_;
    /** Account transaction memo input widget */
    QLineEdit * account_memo_;
    /** Account transaction cleared input widget */
    QCheckBox * account_cleared_;
    /** Add/save account split button */
    QPushButton * account_split_add_;
    /** Clear account split button */
    QPushButton * account_split_clear_;
    /** Delete account split button */
    QPushButton * account_split_delete_;
    /** Account transactions table model */
    AccountSplitModel * account_splits_;
    /** Account transactions table */
    QTableView * account_split_table_;

    /** Envelope selection widget */
    QComboBox * envelope_;
    /** Envelope transaction amount input widget */
    DoubleLineEdit * envelope_amount_;
    /** Envelope transaction memo input widget */
    QLineEdit * envelope_memo_;
    /** Add/save envelope split button */
    QPushButton * envelope_split_add_;
    /** Clear envelope split button */
    QPushButton * envelope_split_clear_;
    /** Delete envelope split button */
    QPushButton * envelope_split_delete_;
    /** Envelope transactions table model */
    EnvelopeSplitModel * envelope_splits_;
    /** Envelope transactions table */
    QTableView * envelope_split_table_;

    /** Button box */
    QDialogButtonBox * buttons_;

    /** Current journal entry */
    std::shared_ptr<JournalEntry> entry_;
    /** Transaction */
    Transaction transaction_;
    /** Current account split */
    AccountTransaction account_split_;
    /** Current envelope split */
    EnvelopeTransaction envelope_split_;

    /**
     * Populates the account selection widget.
     */
    void populateAccountComboBox();

    /**
     * Populates the envelope selection widget.
     */
    void populateEnvelopeComboBox();
};

} // ledger namespace
} // ub namespace
