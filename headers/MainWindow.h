#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "NoteListDelegate.h"
#include "SortFilterProxyModelNotes.h"

#include <QList>
#include <QMainWindow>
#include <QSqlTableModel>
#include <headers/SideBarDelegate.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resizeEvent(QResizeEvent *event) override;

private slots:
    // menu
    void on_pushButtonMenu_clicked();

    void actionAddNoteListHandler();
    void actionNewLoadDatabaseHandler();
    void actionStayOnTopToggledHandler(const bool &checked);
    void actionAboutHandler();
    void actionAboutQtHandler();

    void on_lineEditSearch_textEdited(const QString &str);

    void on_listViewSideBar_clicked(const QModelIndex &index);

    void on_pushButtonAddNote_clicked();

    void removeNoteList(const QModelIndex &index);
    void onNoteDeleted();
    void onNoteFilterChanged();

signals:
    void changeNoteListFilter(int id);
    void changeNoteContentFilter(QString content);

private:
    Ui::MainWindow *ui;

    NoteListDelegate *noteDelegate;
    SortFilterProxyModelNotes *proxyModelNotes;

    QSortFilterProxyModel *proxyModelNoteLists;
    SideBarDelegate *sideBarDelegate;

    void layOutNotes();

    // db
    QSqlDatabase currentDb;
    bool dbConnOpen();
    void dbConnClose();
    bool initDB();

    void changeDatabase(const QString &path);
    void loadDatabase();

    bool removeListsNotes(const int listId);

    QSqlTableModel *dbModelNotes;
    QSqlTableModel *dbModelNoteLists;

    // actions
    void initActions();

    QAction *actionStayOnTop;
    QAction *actionAddNoteList;
    QAction *actionNewLoadDatabase;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QAction *actionQuit;

    // dialog
    void openDBDialog();
    void openNoteListCreationDialog();

    // settings
    QVariant loadSetting(const QString &key);
    void saveSetting(const QString &key, const QVariant &value);
    void setupSettings();

    struct Setting {
         QString name;
         QVariant value;
         Setting(QString name, QVariant value) : name(name), value(value)
         {
         }
    };
    QList<Setting> defaultSettings = QList<Setting>()
            << Setting("AlwaysOnTop", false)
            << Setting("DBPath", QString(""));

};
#endif // MAINWINDOW_H
