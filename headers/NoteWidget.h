#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QDateTime>
#include <QWidget>

namespace Ui {
class NoteWidget;
}

class NoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteWidget(QWidget *parent = nullptr);
    ~NoteWidget();

    void updateTimestamp(const QDateTime &time = QDateTime::currentDateTime());
    QDateTime getTimestamp() const;

    QString getText();
    void setText(const QString &text);

    void focusInEvent(QFocusEvent *event) override;

    bool stillEditing();

signals:
    void textEdited();
    void deleteNote();

private slots:
    void on_plainTextEditNote_textChanged();

    void on_pushButtonDeleteNote_clicked();

    void on_pushButtonCopyNote_clicked();

private:
    Ui::NoteWidget *ui;

    QDateTime timestamp;

    void setTextEditColor(const QString &str);

    QPair<bool, QString> findHexMatch(const QString &str);
    QPair<bool, QString> findRgbMatch(const QString &str);

    bool isDarkColor(const QString &str);
};

#endif // NOTEWIDGET_H
