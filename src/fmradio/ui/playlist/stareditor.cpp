/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   stareditor.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "stareditor.h"
#include <QPainter>

StarEditor::StarEditor(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
}

void StarEditor::setImages(QImage img1, QImage img2)
{
    mImgTrue  = img1;
    mImgFalse = img2;
}

bool StarEditor::value() const
{
    return mValue;
}

void StarEditor::setValue(bool value)
{
    mValue = !value;
    emit editingFinished();
}

void StarEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    painter.drawImage(0, 0, mValue ? mImgTrue : mImgFalse);

    QRect r(QPoint(0, 0), QSize(16, 16));
    r.translate(3, (height() - 16) / 2);
    painter.drawImage(r, mValue ? mImgTrue : mImgFalse);
}

void StarEditor::mouseReleaseEvent(QMouseEvent *event)
{
    mValue = !mValue;
    emit editingFinished();
}

