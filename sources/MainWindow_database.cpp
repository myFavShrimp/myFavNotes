#include "headers/MainWindow.h"

#include <QSqlQuery>

bool MainWindow::initDB()
{
    dbConnClose();
    QList<QString> existingTables;

    if (dbConnOpen()) {
        QSqlQuery query;

        query.exec("SELECT name FROM sqlite_master WHERE type='table'");
        while (query.next()) {
            QString name = query.value(0).toString();
            existingTables << name;
        }

        // table creation
        bool res = true;
        if (!existingTables.contains("myfav_note_lists"))
            if (!query.exec("CREATE TABLE 'myfav_note_lists' ('id' INTEGER, 'name' TEXT NOT NULL, 'position' INTEGER, PRIMARY KEY('id' AUTOINCREMENT))"))
                res = false;
        if (!existingTables.contains("myfav_notes"))
            if (!query.exec("CREATE TABLE 'myfav_notes' ('id' INTEGER, 'list_id' INTEGER NOT NULL, 'content' TEXT, 'timestamp' INTEGER NOT NULL, PRIMARY KEY('id' AUTOINCREMENT))"))
                res = false;

        return res;
    } else
        return false;
}

bool MainWindow::dbConnOpen()
{
    qDebug() << "opening DB";
    currentDb = QSqlDatabase::addDatabase("QSQLITE");
    currentDb.setDatabaseName(settings::loadSetting("DBPath").toString());

    if(!currentDb.open()) {
        return false;
    } else {
        return true;
    }
}

void MainWindow::dbConnClose()
{
    qDebug() << "closing DB";
    if (currentDb.isOpen()) {
        proxyModelNotes->setSourceModel(nullptr);
        delete dbModelNoteLists;
        delete dbModelNotes;
        currentDb.close();
        currentDb = QSqlDatabase();
    }
    currentDb.removeDatabase(QSqlDatabase::defaultConnection);
}

bool MainWindow::removeListsNotes(const int listId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM 'myfav_notes' WHERE list_id=:id");
    query.bindValue(":id", listId);
    qDebug() << query.exec();
    return true;
}
