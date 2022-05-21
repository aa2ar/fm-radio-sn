/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   logmodeldelegate.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "logmodeldelegate.h"
#include <QPainter>

LogModelDelegate::LogModelDelegate(LogModel * m, const QFont & font, const QPalette & pal, QObject *parent)
    : QStyledItemDelegate(parent)
    , mModel(m)
    , mFont(font)
    , mPalette(pal)
{
    mLineHeight = QFontMetrics(mFont).height();
}

void LogModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.row() < mModel->mItems.size()) {
        LogModel::Item & item = mModel->mItems[index.row()];

        painter->setRenderHint(QPainter::TextAntialiasing, true);

        QColor cBr, cPen;
        initObjects(item.type, cBr, cPen);

        QBrush br(cBr);
        QPen pen(cPen);

        painter->setPen(pen);

        painter->fillRect(option.rect, br);

        int ic = item.data.size();
        QRect r{2, option.rect.top(), option.rect.width()-2, mLineHeight};
        for (int i = 0; i < ic; ++i) {
            painter->drawText(r, Qt::AlignLeft|Qt::AlignVCenter, item.data[i]);
            r.translate(0, r.height());
        }
    }
}

QSize LogModelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (0 <= index.row() && index.row() < mModel->mItems.size()) {
        LogModel::Item & item = mModel->mItems[index.row()];
        QSize sz { option.rect.width(), int(item.data.size() * mLineHeight)};
        return sz;
    }
    return {0, 0};
}

void LogModelDelegate::initObjects(LogModel::Item::Type type, QColor & br, QColor & pen) const
{
    switch (type) {
    case LogModel::Item::INPUT:
        br  = mPalette.color(QPalette::Link);
        pen = mPalette.color(QPalette::LinkVisited);
        break;
    case LogModel::Item::OUTPUT:
        br  = mPalette.color(QPalette::Base);
        pen = mPalette.color(QPalette::Text);
        break;
    case LogModel::Item::INFO:
        br  = mPalette.color(QPalette::Highlight);
        pen = mPalette.color(QPalette::HighlightedText);
        break;
    }
}

