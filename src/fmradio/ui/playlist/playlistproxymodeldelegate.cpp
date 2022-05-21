/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistproxymodeldelegate.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "playlistproxymodeldelegate.h"

#include <QPainter>


PlayListProxyModelDelegate::PlayListProxyModelDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void PlayListProxyModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColor co = (index.row() % 2) ? "#E8E8E8" : "#F0F0F0";
    QString s = index.data().toString();
    painter->fillRect(option.rect, co);
    painter->drawText(option.rect.adjusted(2, 2, -2, -2), Qt::AlignLeft|Qt::AlignVCenter, s);
}

QSize PlayListProxyModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return {option.rect.width(), 18};
}

