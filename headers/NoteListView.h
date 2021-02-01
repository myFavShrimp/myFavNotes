#ifndef NOTELISTVIEW_H
#define NOTELISTVIEW_H

#include <QListView>
#include <QWheelEvent>

class NoteListView : public QListView
{
    Q_OBJECT
public:
    explicit NoteListView(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *e) override;

};

#endif // NOTELISTVIEW_H
