#include "headers/TextInputDialog.h"
#include "ui_TextInputDialog.h"

#include <QPushButton>

TextInputDialog::TextInputDialog(QString title, QString labelText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label->setText(labelText);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

QString TextInputDialog::getText()
{
    return ui->lineEditListName->text();
}

void TextInputDialog::on_lineEditListName_textEdited(const QString &text)
{
    if (text.isEmpty())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void TextInputDialog::setText(const QString &text)
{
    ui->lineEditListName->setText(text);
}
