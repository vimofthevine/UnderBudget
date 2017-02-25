// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "icons.hpp"
#include "MenuBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent) {
    open_ = new QAction(Icon::openDocument(), tr("&Open..."), this);
    open_->setShortcut(QKeySequence::Open);
    open_->setStatusTip(tr("Open database"));
    connect(open_, &QAction::triggered, this, &MenuBar::openDatabase);

    exit_ = new QAction(Icon::exit(), tr("E&xit"), this);
    exit_->setShortcut(QKeySequence::Quit);
    exit_->setStatusTip(tr("Quit the application"));
    connect(exit_, &QAction::triggered, this, &MenuBar::exitApplication);

    about_ = new QAction(Icon::about(), tr("&About %1").arg(qApp->applicationName()), this);
    about_->setStatusTip(tr("About the application"));
    connect(about_, &QAction::triggered, this, &MenuBar::aboutApplication);

    about_qt_ = new QAction(QIcon(":/icons/aboutQt"), tr("About &Qt"), this);
    about_qt_->setStatusTip(tr("About the Qt library"));
    connect(about_qt_, &QAction::triggered, this, &MenuBar::aboutQt);

    auto file = addMenu(tr("&File"));
    file->addAction(open_);
    file->addSeparator();
    file->addAction(exit_);

    addSeparator();

    auto help = addMenu(tr("&Help"));
    help->addAction(about_);
    help->addAction(about_qt_);
}

} // ub namespace
