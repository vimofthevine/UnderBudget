// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "MenuBar.hpp"
#include "icons.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MenuBar::MenuBar(QWidget * parent) : QMenuBar(parent) {
    auto open = new QAction(Icon::openDocument(), tr("&Open..."), this);
    open->setShortcut(QKeySequence::Open);
    open->setStatusTip(tr("Open database"));
    connect(open, &QAction::triggered, this, &MenuBar::openDatabase);

    auto exit = new QAction(Icon::exit(), tr("E&xit"), this);
    exit->setShortcut(QKeySequence::Quit);
    exit->setStatusTip(tr("Quit the application"));
    connect(exit, &QAction::triggered, this, &MenuBar::exitApplication);

    auto addtrn = new QAction(tr("&Add Transaction..."), this);
    addtrn->setShortcut(QKeySequence::New);
    addtrn->setStatusTip(tr("Create a new transaction"));
    connect(addtrn, &QAction::triggered, this, &MenuBar::addTransaction);

    auto accounts = new QAction(tr("&Accounts"), this);
    accounts->setShortcut(QKeySequence(Qt::Key_F5));
    accounts->setStatusTip(tr("View Accounts"));
    connect(accounts, &QAction::triggered, this, &MenuBar::viewAccounts);

    auto envelopes = new QAction(tr("&Envelopes"), this);
    envelopes->setShortcut(QKeySequence(Qt::Key_F6));
    envelopes->setStatusTip(tr("View Envelopes"));
    connect(envelopes, &QAction::triggered, this, &MenuBar::viewEnvelopes);

    auto about = new QAction(Icon::about(), tr("&About %1").arg(qApp->applicationName()), this);
    about->setStatusTip(tr("About the application"));
    connect(about, &QAction::triggered, this, &MenuBar::aboutApplication);

    auto aboutqt = new QAction(QIcon(":/icons/aboutQt"), tr("About &Qt"), this);
    aboutqt->setStatusTip(tr("About the Qt library"));
    connect(aboutqt, &QAction::triggered, this, &MenuBar::aboutQt);

    auto file = addMenu(tr("&File"));
    file->addAction(open);
    file->addSeparator();
    file->addAction(exit);

    auto edit = addMenu(tr("&Edit"));
    edit->addAction(addtrn);

    auto view = addMenu(tr("&View"));
    view->addAction(accounts);
    view->addAction(envelopes);

    addSeparator();

    auto help = addMenu(tr("&Help"));
    help->addAction(about);
    help->addAction(aboutqt);
}

} // ub namespace
