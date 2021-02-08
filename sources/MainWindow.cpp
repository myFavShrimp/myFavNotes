#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "TextInputDialog.h"
#include "DBDialog.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings::setupSettings();

    ui->pushButtonAddNote->setDisabled(true);

    // actions
    initActions();

    // view
    sideBarDelegate = new SideBarDelegate(this);
    noteDelegate = new NoteListDelegate(this);
    proxyModelNotes = new SortFilterProxyModelNotes(this);
    proxyModelNoteLists = new QSortFilterProxyModel(this);

    ui->listViewNoteList->setItemDelegate(noteDelegate);
    ui->listViewSideBar->setItemDelegate(sideBarDelegate);
    ui->listViewNoteList->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->listViewNoteList->setModel(proxyModelNotes);
    ui->listViewSideBar->setModel(proxyModelNoteLists);

    ui->listViewNoteList->setResizeMode(QListView::Adjust);
    ui->listViewNoteList->setViewMode(QListView::IconMode);

    connect(ui->listViewSideBar, SIGNAL(removeNoteList(QModelIndex)), this, SLOT(removeNoteList(const QModelIndex)));
    connect(noteDelegate, SIGNAL(noteDeleted()), this, SLOT(onNoteDeleted()));
    connect(proxyModelNotes, SIGNAL(filterChanged()), this, SLOT(onNoteFilterChanged()));

    proxyModelNotes->sort(3, Qt::DescendingOrder);
    proxyModelNoteLists->sort(0, Qt::DescendingOrder);

    // setup
    QFile styleFile(":/resources/style.qss");
    styleFile.open( QFile::ReadOnly);
    QString style(styleFile.readAll());
    setStyleSheet(style);

    if (settings::loadSetting(settings::Setting::DBPath).toString().isEmpty()
            || !QFile(settings::loadSetting(settings::Setting::DBPath).toString()).exists()) {
        openDBDialog();
    } else {
        loadDatabase();
    }
}

MainWindow::~MainWindow()
{
    dbConnClose();
    delete ui;
}

void MainWindow::openDBDialog()
{
    QString path = settings::loadSetting(settings::Setting::DBPath).toString();
    DBDialog dbDialog(path, this);

    if (dbDialog.exec() == QDialog::Accepted) {
        changeDatabase(dbDialog.getPath());
    }
}

void MainWindow::openNoteListCreationDialog()
{
    TextInputDialog inputDialog("New Note List", "Note List Name:", this);
    if (inputDialog.exec() == QDialog::Accepted) {
        qDebug() << "creating note list";
        QSqlRecord listRecord = dbModelNoteLists->record();

        // not setting id manually will result in not being able to edit entry
        listRecord.setValue("id", dbModelNoteLists->rowCount() + 1);
        listRecord.setValue("name", inputDialog.getText());

        dbModelNoteLists->insertRecord(dbModelNoteLists->rowCount(), listRecord);
    }
}

void MainWindow::openChangeTimestampFormatDialog()
{
    TextInputDialog inputDialog("Change Timestamp Format", "Timestamp Format:", this);
    inputDialog.setText(settings::loadSetting(settings::Setting::TimestampFormat).toString());
    if (inputDialog.exec() == QDialog::Accepted)
        settings::saveSetting(settings::Setting::TimestampFormat, inputDialog.getText());
}

void MainWindow::changeDatabase(const QString &path)
{
    settings::saveSetting(settings::Setting::DBPath, path);
    loadDatabase();
}

void MainWindow::loadDatabase()
{
    if (initDB()) {
        qDebug() << "DB loaded successfully";

        dbModelNoteLists = new QSqlTableModel(this);
        dbModelNoteLists->setTable("myfav_note_lists");
        dbModelNoteLists->setEditStrategy(QSqlTableModel::OnFieldChange);
        dbModelNoteLists->sort(0, Qt::DescendingOrder);
        dbModelNoteLists->select();

        dbModelNotes = new QSqlTableModel(this);
        dbModelNotes->setTable("myfav_notes");
        dbModelNotes->setEditStrategy(QSqlTableModel::OnFieldChange);
        dbModelNotes->select();

        proxyModelNotes->setSourceModel(dbModelNotes);
        proxyModelNoteLists->setSourceModel(dbModelNoteLists);

        ui->listViewSideBar->setModelColumn(1);

        connect(this, SIGNAL(changeNoteContentFilter(QString)), proxyModelNotes, SLOT(setNoteContentFilter(const QString)));
        connect(this, SIGNAL(changeNoteListFilter(int)), proxyModelNotes, SLOT(setNoteListFilter(const int)));

    } else {
        dbConnClose();
        QMessageBox::warning(this, "Warning", "Database couldn't be created / loaded!");
    }
}

void MainWindow::on_lineEditSearch_textEdited(const QString &str)
{
    emit changeNoteContentFilter(str);
}

void MainWindow::on_listViewSideBar_clicked(const QModelIndex &index)
{
    emit changeNoteListFilter(index.sibling(index.row(), 0).data().toInt()-1);
    ui->pushButtonAddNote->setDisabled(false);
}


void MainWindow::on_pushButtonAddNote_clicked()
{
    QSqlRecord myrecord = dbModelNotes->record();
    QModelIndex sideBarIndex = ui->listViewSideBar->currentIndex().sibling(ui->listViewSideBar->currentIndex().row(), 0);
    int listId = sideBarIndex.data().toInt();

    myrecord.remove(0);
    myrecord.setValue("timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());
    myrecord.setValue("list_id", listId);
    myrecord.setValue("content", "");

    if (!dbModelNotes->insertRecord(dbModelNotes->rowCount(), myrecord))
        qDebug() << dbModelNotes->lastError();

    dbModelNotes->select();

    layOutNotes();

    ui->listViewNoteList->scrollToTop();
}

void MainWindow::removeNoteList(const QModelIndex &index)
{
    if (removeListsNotes(index.siblingAtColumn(0).data().toInt())) {
        proxyModelNoteLists->removeRow(index.row(), index.parent());
        dbModelNoteLists->select();
        dbModelNotes->select();
        proxyModelNotes->setNoteListFilter(-1);
        ui->pushButtonAddNote->setDisabled(true);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    layOutNotes();
}

void MainWindow::onNoteDeleted()
{
    layOutNotes();
}

void MainWindow::onNoteFilterChanged()
{
    layOutNotes();
}

void MainWindow::layOutNotes()
{
    int maximumWidth = 200;
    int currentWidth = ui->listViewNoteList->width();
    int currentItemCount = proxyModelNotes->rowCount();

    if (currentWidth >= maximumWidth) { // prevent crash caused by small initial size
        int horItemCount = currentWidth / maximumWidth;

        if (currentItemCount && horItemCount > currentItemCount)
            horItemCount = currentItemCount;

        int widthPerItem  = horItemCount * maximumWidth;
        int remainingWidth = currentWidth - widthPerItem;
        int itemWidth = maximumWidth + ((remainingWidth / horItemCount) - 3);
        if (!(ui->listViewNoteList->gridSize() == QSize(itemWidth, 100)))
            ui->listViewNoteList->setGridSize(QSize(itemWidth, 100));
    }
}
