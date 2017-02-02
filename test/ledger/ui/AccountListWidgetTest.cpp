// Standard include(s)
#include <memory>

// Qt include(s)
#include <QApplication>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/persistence/SQLAccountRepository.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include <ledger/persistence/SQLEnvelopeRepository.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>
#include <ledger/ui/AccountListWidget.hpp>
#include <ledger/ui/AccountModel.hpp>

int main(int argc, char *argv[]) {
    using namespace ub::ledger;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    db.open();

    SQLCurrencyRepository currencies(db);
    auto accounts = std::make_shared<SQLAccountRepository>(db);
    auto envelopes = std::make_shared<SQLEnvelopeRepository>(db);
    auto transactions = std::make_shared<SQLTransactionRepository>(db, accounts, envelopes);

    {
        Account acct;
        acct.setName("Parent");
        accounts->create(acct, accounts->getRoot());

        acct.setName("Child 1");
        accounts->create(acct, accounts->getAccount(2));
        acct.setName("Child 2");
        accounts->create(acct, accounts->getAccount(2));

        acct.setName("Grandchild 1");
        accounts->create(acct, accounts->getAccount(4));

        acct.setName("Grandchild 2");
        accounts->create(acct, accounts->getAccount(3));
    }

    QApplication app(argc, argv);
    auto model = new AccountModel(accounts, transactions);
    auto view = new AccountListWidget(model, nullptr);
    view->show();
    return app.exec();
}
