#include "headers/DBDialog.h"
#include "ui_DBDialog.h"

#include <QFileDialog>
#include <QStandardPaths>

DBDialog::DBDialog(QString &dbPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    path = dbPath;

    if (dbPath.isEmpty()) {
        docDir.setPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        path = docDir.filePath("myFavNotesDB.db");
    }
    ui->labelPath->setText(path);
}

DBDialog::~DBDialog()
{
    delete ui;
}

QString DBDialog::getPath()
{
    return path;
}

void DBDialog::on_pushButtonChoosePath_clicked()
{
    path = QFileDialog::getSaveFileName(this, tr("Select Folder"),
                                        ui->labelPath->text(),
                                        "SQLite Database (*.db)",
                                        nullptr, QFileDialog::DontConfirmOverwrite);
    if (path.isEmpty()) {
        docDir.setPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        path = docDir.filePath("myFavNotesDB.db");
    }
    ui->labelPath->setText(path);
}
