#include "headers/NoteListDelegate.h"
#include "headers/SortFilterProxyModelNotes.h"

#include <QPainter>

NoteListDelegate::NoteListDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
    paintWidget = new NoteWidget(parent);
    // hiding the widget to not render it on top of the first delegate
    paintWidget->hide();
    deleteNoteRow = false;
}

NoteListDelegate::~NoteListDelegate()
{
    delete paintWidget;
}

QWidget *NoteListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    NoteWidget *editor = new NoteWidget(parent);
    connect(editor, SIGNAL(textEdited()), this, SLOT(noteEdited()));
    connect(editor, SIGNAL(deleteNote()), this, SLOT(deleteNote()));
    lastEditorCreated = editor;
    lastIndex = new QModelIndex(index);
    return editor;
}

void NoteListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (editor == lastEditorEdited)
        return; // TextCursor will be reset otherwise

    QMap data = index.model()->data(index, Qt::EditRole).toMap();
    QString content = data["content"].toString();
    QDateTime time = QDateTime::fromSecsSinceEpoch(data["time"].toInt());
    NoteWidget *note = static_cast<NoteWidget*>(editor);

    note->setText(content);
    note->updateTimestamp(time);
}

void NoteListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    NoteWidget *note = static_cast<NoteWidget*>(editor);
    QString value = note->getText();

    if (/*(!note->stillEditing() && value.isEmpty()) ||*/ deleteNoteRow) { // causes unselectable notes
        model->removeRow(index.row(), index.parent());
        deleteNoteRow = false;
        return;
    }

    QMap<QString, QVariant> values;
    qint64 time = note->getTimestamp().toSecsSinceEpoch();

    values.insert("content", value);
    values.insert("time", time);

    model->setData(index, QVariant(values), Qt::EditRole);
}

void NoteListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

void NoteListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QMap data = index.model()->data(index, Qt::EditRole).toMap();
    QString value = data["content"].toString();
    QDateTime time = QDateTime::fromSecsSinceEpoch(data["time"].toInt());

    paintWidget->setText(value);
    paintWidget->updateTimestamp(time);
    paintWidget->resize(option.rect.size());

    paintWidget->setStyleSheet("");

    painter->translate(option.rect.topLeft());
    paintWidget->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    paintWidget->setText(value);

    painter->restore();
}

QSize NoteListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(0, 92);
}

void NoteListDelegate::noteEdited()
{
    lastEditorEdited = lastEditorCreated;
    emit commitData(lastEditorCreated);
}

void NoteListDelegate::deleteNote()
{
    deleteNoteRow = true;
    emit commitData(lastEditorCreated);
}
