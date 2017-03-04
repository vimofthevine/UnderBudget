#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QObject>
#include <QString>

namespace ub {

// Forward declaration(s)
class MainWindow;
class Repositories;

namespace ledger {
// Forward declaration(s)
class AccountListWidget;
class AccountModel;
class AccountTransactionModel;
class EnvelopeListWidget;
class EnvelopeModel;
class EnvelopeTransactionModel;
class JournalEntryDialog;
}

/**
 * Main application window model.
 */
class MainWindowModel : public QObject {
    Q_OBJECT

public:
    /**
     * Initializes the main window model.
     *
     * @param window Main window
     */
    MainWindowModel(MainWindow *window);

    /**
     * Updates the repositories used by the application.
     *
     * @param repositories
     */
    void setRepositories(std::shared_ptr<Repositories> repositories);

protected slots:
    /**
     * Prompts the user to select a database file to be opened.
     */
    void openDatabase();

    /**
     * Displays the accounts list view.
     */
    void showAccounts();

    /**
     * Displays the envelopes list view.
     */
    void showEnvelopes();

    /**
     * Displays an error popup dialog with the given message.
     *
     * @param message Error message
     */
    void showError(const QString &message);

private:
    /** Account model */
    ledger::AccountModel *account_model_;
    /** Account transaction model */
    ledger::AccountTransactionModel *account_transaction_model_;
    /** Envelope model */
    ledger::EnvelopeModel * envelope_model_;
    /** Envelope transaction model */
    ledger::EnvelopeTransactionModel * envelope_transaction_model_;

    /** Main window */
    MainWindow *window_;
    /** Account list widget */
    ledger::AccountListWidget *account_list_;
    /** Envelope list widget */
    ledger::EnvelopeListWidget * envelope_list_;
    /** Journal entry dialog */
    ledger::JournalEntryDialog * journal_entry_;
};

} // ub namespace
