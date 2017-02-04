// Standard include(s)
#include <string>

// Qt include(s)
#include <QtCore>
#include <QtWidgets>

// UnderBudget include(s)
#include "DatabaseFileChooser.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
const QString LAST_OPENED_DB = "LastOpenedDatabase";
const QString LAST_USED_DIR = "LastUsedDatabaseDir";
char const * const DB_FILE_FILTER = "UnderBudget Database (*.budget);;All (*)";

//--------------------------------------------------------------------------------------------------
QString DatabaseFileChooser::lastOpenedFile() {
    QSettings settings;
    auto last = settings.value(LAST_OPENED_DB).toString();
    QFileInfo info(last);
    qDebug() << "Last opened DB file:" << last << "exists:" << info.exists();
    return info.exists() ? last : "";
}

//--------------------------------------------------------------------------------------------------
QString DatabaseFileChooser::getFileToOpen(QWidget *parent) {
    QSettings settings;
    QString name = QFileDialog::getOpenFileName(parent,
                                                QObject::tr("Open Database File"),
                                                settings.value(LAST_USED_DIR).toString(),
                                                QObject::tr(DB_FILE_FILTER));
    if (not name.isEmpty()) {
        QFileInfo info(name);
        qDebug() << "Selected DB file:" << name << "exists:" << info.exists();
        auto dir = info.exists() ? info.canonicalPath() : info.absolutePath();
        settings.setValue(LAST_USED_DIR, dir);

        if (info.exists()) {
            settings.setValue(LAST_OPENED_DB, info.canonicalFilePath());
        }
    }

    return name;
}

} // ub namespace
