#include "headers/SortFilterProxyModelNotes.h"

#include <QCryptographicHash>

SortFilterProxyModelNotes::SortFilterProxyModelNotes(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

QVariant SortFilterProxyModelNotes::data(const QModelIndex &proxyIndex, int role) const
{
    QMap<QString, QVariant> values;
    int row = proxyIndex.row();

    QModelIndex index = proxyIndex.sibling(row, 2); // get content col
    values.insert("content", sourceModel()->data(mapToSource(index), role).toString());

    index = proxyIndex.sibling(row, 3); // get time col
    values.insert("time", sourceModel()->data(mapToSource(index), role).toInt());

    return values;
}

bool SortFilterProxyModelNotes::setData(const QModelIndex &proxyIndex, const QVariant &value, int role)
{
    QMap<QString, QVariant> data = value.toMap();
    QString text = data["content"].toString();
    qint64 time = data["time"].toInt();

    int row = proxyIndex.row();
    bool ret = true;

    QModelIndex index = proxyIndex.sibling(row, 2); // set content col
    if (!sourceModel()->setData(mapToSource(index), text, role))
        ret = false;

    index = proxyIndex.sibling(row, 3); // set time col
    if (!sourceModel()->setData(mapToSource(index), time, role))
        ret = false;

    return ret;
}

bool SortFilterProxyModelNotes::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex noteListIdIndex = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex contentIndex = sourceModel()->index(source_row, 2, source_parent);

    int noteListId = sourceModel()->data(noteListIdIndex).toInt();
    QString content = sourceModel()->data(contentIndex).toString();

    return (noteListId == noteListFilter
            && content.contains(noteContentFilter, Qt::CaseInsensitive));
}

void SortFilterProxyModelNotes::setNoteListFilter(const int &id)
{
    beginResetModel();
    noteListFilter = id+1;
    endResetModel();

    emit filterChanged();
}

void SortFilterProxyModelNotes::setNoteContentFilter(const QString &content)
{
    beginResetModel();
    noteContentFilter = content;
    endResetModel();

    emit filterChanged();
}

int SortFilterProxyModelNotes::getNoteListFilter() const
{
    return noteListFilter;
}
