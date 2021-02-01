#ifndef DBDIALOG_H
#define DBDIALOG_H

#include <QDialog>
#include <QDir>

namespace Ui {
class DBDialog;
}

class DBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBDialog(QString &dbPath, QWidget *parent = nullptr);
    ~DBDialog();

    QString getPath();

private slots:
    void on_pushButtonChoosePath_clicked();

private:
    Ui::DBDialog *ui;

    QDir docDir;
    QString path;
};

#endif // DBDIALOG_H
