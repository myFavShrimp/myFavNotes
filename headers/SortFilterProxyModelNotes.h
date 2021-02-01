#ifndef SORTFILTERPROXYMODELNOTES_H
#define SORTFILTERPROXYMODELNOTES_H

#include <QSortFilterProxyModel>

class SortFilterProxyModelNotes : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModelNotes(QObject *parent = 0);

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    int getNoteListFilter() const;

public slots:
    void setNoteListFilter(const int &id);
    void setNoteContentFilter(const QString &content);

private:
    int noteListFilter;
    QString noteContentFilter;

};

#endif // SORTFILTERPROXYMODELNOTES_H
