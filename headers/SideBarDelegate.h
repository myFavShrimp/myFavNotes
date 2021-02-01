#ifndef SIDEBARDELEGATE_H
#define SIDEBARDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>

class SideBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SideBarDelegate(QWidget *parent = nullptr);
    ~SideBarDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString displayText(const QVariant &value, const QLocale &locale) const override;

private:
    QPushButton *paintWidget;
};

#endif // SIDEBARDELEGATE_H
