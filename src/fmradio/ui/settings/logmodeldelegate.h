/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   logmodeldelegate.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef LOGMODELDELEGATE_H
#define LOGMODELDELEGATE_H

#include <QStyledItemDelegate>
#include "logmodel.h"

class LogModel;
class LogModelDelegate : public QStyledItemDelegate
{
public:
    LogModelDelegate(LogModel *, const QFont &, const QPalette &, QObject * parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    LogModel * mModel = nullptr;
    QFont mFont;
    QPalette mPalette;
    int mLineHeight = 0;
    void initObjects(LogModel::Item::Type, QColor &, QColor &) const;
};

#endif // LOGMODELDELEGATE_H
