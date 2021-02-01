#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class TextInputDialog;
}

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextInputDialog(QString title, QString labelText, QWidget *parent = nullptr);
    ~TextInputDialog();

    QString getText();

private slots:
    void on_lineEditListName_textEdited(const QString &text);

private:
    Ui::TextInputDialog *ui;
};

#endif // TEXTINPUTDIALOG_H
