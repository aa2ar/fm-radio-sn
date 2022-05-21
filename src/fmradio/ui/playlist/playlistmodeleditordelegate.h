/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistmodeleditordelegate.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef PLAYLISTMODELEDITORDELEGATE_H
#define PLAYLISTMODELEDITORDELEGATE_H

#include <QStyledItemDelegate>

class PlayListModelEditorDelegate : public QStyledItemDelegate
{
public:
    explicit PlayListModelEditorDelegate(QFont, QObject * parent = 0);

    void paint(QPainter * painter,
               const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option,
                             const QModelIndex & index) const override;
    void setEditorData(QWidget * editor, const QModelIndex & index) const override;
    void setModelData(QWidget * editor, QAbstractItemModel * model,
                      const QModelIndex &index) const override;

private:
    QImage mImgTrue;
    QImage mImgFalse;
    QImage mImgChecked;
    QImage mImgUnchecked;
    QFont  mFont;
    int mRowHeight = 24;

    QColor mColor = QColor("lightgray");
    QColor mSelColor = QColor("gray");

    void commitAndCloseEditor();
};

#endif // PLAYLISTMODELEDITORDELEGATE_H
