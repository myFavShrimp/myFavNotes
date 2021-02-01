#include "headers/SideBarListView.h"

#include <QMenu>
#include <QMouseEvent>

SideBarListView::SideBarListView(QWidget *parent) : QListView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    actionRemoveNoteList = new QAction("Delete Note List", this);
    label = new QLabel(this);

    label->setAlignment(Qt::AlignCenter);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
    connect(actionRemoveNoteList, SIGNAL(triggered()), this, SLOT(removeListClicked()));
}

void SideBarListView::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
    return;
}

void SideBarListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.count() == 0 && deselected.count()) {
        setCurrentIndex(deselected.indexes().takeFirst());
    }
}

void SideBarListView::showContextMenu(const QPoint &pos)
{
    contextItem = new QModelIndex(indexAt(pos));

    if (!contextItem->isValid())
        return;

    contextMenu = new QMenu(tr("Context menu"), this);

    emit clicked(*contextItem);

    label->setText(tr("<b>%1</b>").arg(contextItem->data().toString()));

    act = new QWidgetAction(contextMenu);
    act->setDefaultWidget(label);
    contextMenu->addAction(act);

    contextMenu->addAction(actionRemoveNoteList);

    contextMenu->exec(mapToGlobal(pos));
}

void SideBarListView::removeListClicked()
{
    emit removeNoteList(*contextItem);
}
