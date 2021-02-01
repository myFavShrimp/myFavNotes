#include "headers/SideBarDelegate.h"

#include <QPainter>

SideBarDelegate::SideBarDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
    paintWidget = new QPushButton(parent);
    paintWidget->hide();
}

SideBarDelegate::~SideBarDelegate()
{
    delete paintWidget;
}

void SideBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem newOpt(option);
    newOpt.font.setPixelSize(15);
    newOpt.font.setWeight(QFont::Bold);
    newOpt.displayAlignment = Qt::AlignCenter;

    QStyledItemDelegate::paint(painter, newOpt, index);
}

QSize SideBarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    int width = option.rect.width();
    int decWidth = option.decorationSize.width() / 4;
    int height = width + decWidth;
    return QSize(width, height);
}

QString SideBarDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale)
    QString result = value.toString().first(1);
    return result;
}
