#include "headers/MainWindow.h"

#include <QAction>
#include <QMenu>
#include <headers/TextInputDialog.h>
#include <QMessageBox>

void MainWindow::initActions()
{
    actionAddNoteList = new QAction("Add Note List", this);
    actionNewLoadDatabase = new QAction("Create New / Load Database", this);
    actionStayOnTop = new QAction("Always On Top", this);
    actionAbout = new QAction("About", this);
    actionAboutQt = new QAction("About Qt", this);
    actionQuit = new QAction("Quit", this);

    actionStayOnTop->setCheckable(true);

    connect(actionAddNoteList, SIGNAL(triggered()), this, SLOT(actionAddNoteListHandler()));
    connect(actionNewLoadDatabase, SIGNAL(triggered()), this, SLOT(actionNewLoadDatabaseHandler()));
    connect(actionStayOnTop, SIGNAL(toggled(bool)), this, SLOT(actionStayOnTopToggledHandler(const bool)));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(actionAboutHandler()));
    connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(actionAboutQtHandler()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    actionStayOnTop->setChecked(settings::loadSetting(settings::Setting::AlwaysOnTop).toBool());
}

void MainWindow::on_pushButtonMenu_clicked()
{
    QMenu contextMenu(tr("Menu"), this);

    actionAddNoteList->setDisabled(!currentDb.isOpen());

    contextMenu.addAction(actionAddNoteList);
    contextMenu.addAction(actionNewLoadDatabase);
    contextMenu.addSeparator();
    contextMenu.addAction(actionStayOnTop);
    contextMenu.addSeparator();
    contextMenu.addAction(actionAbout);
    contextMenu.addAction(actionAboutQt);
    contextMenu.addSeparator();
    contextMenu.addAction(actionQuit);

    contextMenu.exec(mapToParent(mapFromGlobal(QCursor::pos())));
}

void MainWindow::actionAddNoteListHandler()
{
    openNoteListCreationDialog();
}

void MainWindow::actionNewLoadDatabaseHandler()
{
    openDBDialog();
}

void MainWindow::actionStayOnTopToggledHandler(const bool &checked)
{
    bool windowVisible = isVisible();
    Qt::WindowFlags currentFlags = windowFlags();
    if (checked) {
        setWindowFlags(currentFlags | Qt::X11BypassWindowManagerHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    } else
        setWindowFlags(currentFlags ^ (Qt::X11BypassWindowManagerHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
    settings::saveSetting(settings::Setting::AlwaysOnTop, checked);
    if (windowVisible) // prevent crash caused by showing the application before it's fully initialized at startup
        show();
}

void MainWindow::actionAboutHandler() {
    QMessageBox::about(this, "About",
                          "<b>About myFavNotes</b><br><br>"
                          "Application and app icon made by myFavShrimp with ❤.<br>"
                          "All other icons made by <a href=\"https://www.flaticon.com/authors/pixel-perfect\" title=\"Pixel perfect\">Pixel perfect</a> from <a href=\"https://www.flaticon.com/\" title=\"Flaticon\">www.flaticon.com</a>.");
}

void MainWindow::actionAboutQtHandler() {
    QMessageBox::aboutQt(this, "About Qt");
}
