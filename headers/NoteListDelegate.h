#ifndef NOTELISTDELEGATE_H
#define NOTELISTDELEGATE_H

#include "NoteWidget.h"

#include <QStyledItemDelegate>

class NoteListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NoteListDelegate(QWidget *parent = nullptr);
    ~NoteListDelegate();

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private slots:
    void noteEdited();
    void deleteNote();

private:
    NoteWidget *paintWidget;
    mutable NoteWidget *lastEditorCreated;
    mutable NoteWidget *lastEditorEdited;
    mutable bool deleteNoteRow;
    mutable QModelIndex *lastIndex;
};

#endif // NOTELISTDELEGATE_H
