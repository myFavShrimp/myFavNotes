#include "headers/NoteListView.h"

#include <QApplication>

NoteListView::NoteListView(QWidget *parent) : QListView(parent)
{
    setMouseTracking(true);
}

void NoteListView::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    QPoint localCursorPos = mapFromGlobal(QCursor::pos());

    QModelIndex index = indexAt(localCursorPos);
    QRect rect = visualRect(index);
    QPoint topLeft = rect.topLeft();
    QPoint bottomRight = rect.bottomRight();

    if (localCursorPos.x() > topLeft.x()+3 && localCursorPos.y() > topLeft.y()+3
            && localCursorPos.x() < bottomRight.x() && localCursorPos.y() < bottomRight.y()-25)
        setCursor(Qt::IBeamCursor);
    else
        setCursor(Qt::ArrowCursor);
}
