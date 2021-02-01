#ifndef SIDEBARLISTVIEW_H
#define SIDEBARLISTVIEW_H

#include <QLabel>
#include <QListView>
#include <QWidgetAction>

class SideBarListView : public QListView
{
    Q_OBJECT
public:
    explicit SideBarListView(QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void removeNoteList(QModelIndex index);

private slots:
    void showContextMenu(const QPoint &pos);
    void removeListClicked();

private:
    QMenu *contextMenu;
    QLabel *label;
    QWidgetAction *act;
    QAction *actionRemoveNoteList;

    QModelIndex *contextItem;

};

#endif // SIDEBARLISTVIEW_H
