// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "icons.hpp"
#include "MenuBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MenuBar::MenuBar(QMenuBar *menu) {
    open_ = new QAction(Icon::openDocument(), menu->tr("&Open..."), menu);
    open_->setShortcut(QKeySequence::Open);
    open_->setStatusTip(menu->tr("Open database"));

    exit_ = new QAction(Icon::exit(), menu->tr("E&xit"), menu);
    exit_->setShortcut(QKeySequence::Quit);
    exit_->setStatusTip(menu->tr("Quit the application"));

    about_ = new QAction(Icon::about(), menu->tr("&About %1").arg(qApp->applicationName()), menu);
    about_->setStatusTip(menu->tr("About the application"));

    about_qt_ = new QAction(QIcon(":/icons/aboutQt"), menu->tr("About &Qt"), menu);
    about_qt_->setStatusTip(menu->tr("About the Qt library"));

    auto file = menu->addMenu(menu->tr("&File"));
    file->addAction(open_);
    file->addSeparator();
    file->addAction(exit_);

    menu->addSeparator();

    auto help = menu->addMenu(menu->tr("&Help"));
    help->addAction(about_);
    help->addAction(about_qt_);
}

} // ub namespace
