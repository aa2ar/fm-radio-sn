/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistmodeleditordelegate.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "playlistmodeleditordelegate.h"
#include "stareditor.h"
#include <QPainter>
#include <QLineEdit>

PlayListModelEditorDelegate::PlayListModelEditorDelegate(QFont f, QObject * parent)
    : QStyledItemDelegate(parent)
{
    mImgTrue  = QImage(":/rc/img/star_yellow-16x16.png");
    mImgFalse = QImage(":/rc/img/star_gray-16x16.png");
    mImgChecked = QImage(":/rc/img/checked-16x16.png");
    mImgUnchecked = QImage(":/rc/img/unchecked-16x16.png");
    mFont = f;
    mRowHeight = QFontMetrics(mFont).height() + 10;
}

void PlayListModelEditorDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    painter->fillRect(option.rect, (option.state & QStyle::State_Selected) ? mSelColor : mColor);

    switch (index.column()) {
    case 0: {
        QRect r(option.rect.topLeft(), QSize(16, 16));
        r.translate(3, (option.rect.height() - 16) / 2);
        painter->drawImage(r, index.data().toBool() ? mImgChecked : mImgUnchecked);
    } break;

    case 1: {
        QRect r(option.rect.topLeft(), QSize(16, 16));
        r.translate(3, (option.rect.height() - 16) / 2);
        painter->drawImage(r, index.data().toBool() ? mImgTrue : mImgFalse);
    } break;

    case 2: {
        QString s = QString().sprintf("%.1f MHz", index.data().toInt() / 10.0);
        painter->drawText(option.rect, Qt::AlignCenter, s);

    } break;

    case 3:
        painter->drawText(option.rect.translated(3, 0), Qt::AlignLeft|Qt::AlignVCenter, index.data().toString());
        break;
    }
}

QSize PlayListModelEditorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if (index.column() == 0)
        return {24, mRowHeight};

    if (index.column() == 1)
        return {24, mRowHeight};

    return {option.rect.width(), mRowHeight};
}

QWidget * PlayListModelEditorDelegate::createEditor(QWidget * parent,
                                                    const QStyleOptionViewItem & option,
                                                    const QModelIndex & index) const
{
//    if (index.column() == 0) {
//        StarEditor * ed = new StarEditor(parent);
//        ed->setImages(mImgTrue, mImgFalse);
//        connect(ed, &StarEditor::editingFinished,
//                        this, &PlayListModelEditorDelegate::commitAndCloseEditor);
//        return ed;
//    }
    if (index.column() == 3)
        return new QLineEdit(parent);

    return nullptr;
}

void PlayListModelEditorDelegate::setEditorData(QWidget * editor,
                                                const QModelIndex & index) const
{
//    if (index.column() == 0) {
//        StarEditor * ed = static_cast<StarEditor *>(editor);
//        ed->setValue(index.data().toBool());
//    }
//    else
    if (index.column() == 3) {
        QLineEdit * ed = static_cast<QLineEdit *>(editor);
        ed->setText(index.data().toString());
    }
}

void PlayListModelEditorDelegate::setModelData(QWidget * editor,
                                               QAbstractItemModel * model,
                                               const QModelIndex & index) const
{
//    if (index.column() == 0) {
//        StarEditor * ed = static_cast<StarEditor *>(editor);
//        model->setData(index, ed->value(), Qt::DisplayRole);
//    }
    if (index.column() == 3) {
        QLineEdit * ed = static_cast<QLineEdit *>(editor);
        model->setData(index, ed->text(), Qt::DisplayRole);
    }
}

void PlayListModelEditorDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
