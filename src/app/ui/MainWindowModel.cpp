// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <ledger/ui/AccountListWidget.hpp>
#include <ledger/ui/AccountModel.hpp>
#include "DatabaseFileChooser.hpp"
#include "MainWindow.hpp"
#include "MainWindowModel.hpp"
#include "MenuBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MainWindowModel::MainWindowModel(MainWindow *window)
        : QObject(window), account_model_(new ledger::AccountModel), window_(window),
          account_list_(new ledger::AccountListWidget(account_model_, window_)) {
    window_->contentWidget()->addWidget(account_list_);

    connect(window_, &MainWindow::openDatabase, this, &MainWindowModel::openDatabase);
    connect(account_model_, &ledger::AccountModel::error, this, &MainWindowModel::showError);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::setRepositories(std::shared_ptr<Repositories> repositories) {
    account_model_->setRepository(repositories);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::openDatabase() {
    auto file = ub::DatabaseFileChooser::getFileToOpen(window_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showError(const QString &message) {
    QMessageBox::warning(window_, tr("Error"), message);
}

} // ub namespace
