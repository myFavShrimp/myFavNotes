#include "headers/NoteWidget.h"
#include "ui_NoteWidget.h"

#include <QRegularExpression>
#include <QHash>
#include <QClipboard>

NoteWidget::NoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteWidget)
{
    ui->setupUi(this);

    updateTimestamp();

}

NoteWidget::~NoteWidget()
{
    delete ui;
}

void NoteWidget::updateTimestamp(const QDateTime &time)
{
    timestamp = time;
    // TODO make date time format changable
    ui->labelTimestamp->setText(timestamp.toString("dd.MM.yyyy hh:mm:ss"));
}

QString NoteWidget::getText()
{
    return ui->plainTextEditNote->toPlainText();
}

void NoteWidget::setText(const QString &text)
{
    ui->plainTextEditNote->setPlainText(text);
    QPair<bool, QString> hexTestResult = findHexMatch
            (ui->plainTextEditNote->toPlainText());
}

void NoteWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    QPoint localCursorPos = mapFromGlobal(QCursor::pos());

    // was a button clicked ?
    if (ui->horizontalLayoutButtons->geometry().contains(localCursorPos)) {
        int count = ui->horizontalLayoutButtons->count();
        for (int i = 0; i < count; i++) {
            QPushButton *button = static_cast<QPushButton*>(ui->horizontalLayoutButtons->itemAt(i)->widget());
            if (button->geometry().contains(localCursorPos))
                button->animateClick();
        }
    // no button was clicked
    } else {
        QTextCursor cursor = ui->plainTextEditNote->cursorForPosition(localCursorPos);
        ui->plainTextEditNote->setFocus();
        ui->plainTextEditNote->setTextCursor(cursor);
    }
}

bool NoteWidget::stillEditing()
{
    return ui->plainTextEditNote->hasFocus();
}

void NoteWidget::on_plainTextEditNote_textChanged()
{
    updateTimestamp();
    QPair<bool, QString> regExTestResult = findHexMatch(ui->plainTextEditNote->toPlainText());
    if (regExTestResult.first)
        setTextEditColor(regExTestResult.second);
    else {
        regExTestResult = findRgbMatch(ui->plainTextEditNote->toPlainText());
        if (regExTestResult.first)
            setTextEditColor(regExTestResult.second);
        else
            setTextEditColor("#36393f");
    }
    emit textEdited();
}

QDateTime NoteWidget::getTimestamp() const
{
    return timestamp;
}

void NoteWidget::setTextEditColor(const QString &str)
{
    // adjust text color
    if (isDarkColor(str)) {
        setStyleSheet(styleSheet().append(QString("color: %1; ").arg("white")));
    } else {
        setStyleSheet(styleSheet().append(QString("color: %1; ").arg("black")));
    }
    ui->plainTextEditNote->setStyleSheet(styleSheet().append(QString("background-color: %1;")
                                         .arg(str)));
    ui->labelTimestamp->setStyleSheet(styleSheet().append(QString("background-color: %1;")
                                         .arg(str)));
}

QPair<bool, QString> NoteWidget::findHexMatch(const QString &str)
{
    QRegularExpression hexMatcher("#(?:[0-9A-F]{3}){1,2}",
                                  QRegularExpression::CaseInsensitiveOption);
    QPair<bool, QString> ret(false, nullptr);
    if (hexMatcher.match(str).hasMatch()) {
        ret.first = true;
        ret.second = hexMatcher.match(str).captured(0);
    }
    return ret;
}

QPair<bool, QString> NoteWidget::findRgbMatch(const QString &str)
{
    // unescaped: rgba?\((\d{1,3}),\s*(\d{1,3}),\s*(\d{1,3})
    QRegularExpression rgbMatcher("rgba?\\((\\d{1,3}),\\s*(\\d{1,3}),\\s*(\\d{1,3})",
                      QRegularExpression::CaseInsensitiveOption);
    QPair<bool, QString> ret(false, nullptr);
    if (rgbMatcher.match(str).hasMatch()) {
        ret.first = true;
        ret.second = rgbMatcher.match(str).captured(0);

        // add alpha value if rgba
        if (ret.second[3] == 'a')
            ret.second = ret.second.append(",1)");
        else
            ret.second = rgbMatcher.match(str).captured(0).append(")");
    }
    return ret;
}

bool NoteWidget::isDarkColor(const QString &str)
{
    int r;
    int g;
    int b;

    // get rgb values if rgb string - super overcomplicated
    if (str.startsWith('r')) {
        QRegularExpression rgbGetter("\\d{1,3}");
        QRegularExpressionMatchIterator iterator = rgbGetter.globalMatch(str);

        r = iterator.next().captured(0).toInt();
        g = iterator.next().captured(0).toInt();
        b = iterator.next().captured(0).toInt();

    } else {
        QColor color;
        color.setNamedColor(str);
        color.getRgb(&r, &g, &b);
    }

    int luma = 0.2126f * r + 0.7152f * g + 0.0722f * b;
    if (luma > 127)
        return false;
    return true;
}

void NoteWidget::on_pushButtonDeleteNote_clicked()
{
    emit deleteNote();
}

void NoteWidget::on_pushButtonCopyNote_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(getText());
}
