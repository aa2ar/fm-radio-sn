/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistproxymodeldelegate.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef PLAYLISTPROXYMODELDELEGATE_H
#define PLAYLISTPROXYMODELDELEGATE_H

#include <QStyledItemDelegate>

class QPainter;
class PlayListProxyModelDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlayListProxyModelDelegate(QObject * parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;

signals:

public slots:
};

#endif // PLAYLISTPROXYMODELDELEGATE_H
