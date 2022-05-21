/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   radioknob.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "radioknob.h"
//#include <QToolButton>
#include <QLabel>
#include <QWheelEvent>
#include <QIcon>
#include <QDebug>

RadioKnob::RadioKnob(QWidget * parent)
    : TunerKnob(parent)
{}

void RadioKnob::initLabel(QPixmap icon, QPixmap iconHovered)
{
    mPixmaps.normal = icon;
    mPixmaps.hovered = iconHovered;

    if (mLabel == nullptr) {
        mLabel = new QLabel(this);
        mLabel->setAlignment(Qt::AlignCenter);
    }
    mLabel->setPixmap(icon);
    mLabel->setText(QString());
    updateLabelGeometry();
}

void RadioKnob::enableLabel(bool value)
{
    if (mLabel)
        mLabel->setEnabled(value);
}

void RadioKnob::enterEvent(QEvent *)
{
    if (mLabel)
        mLabel->setPixmap(mPixmaps.hovered);
}

void RadioKnob::leaveEvent(QEvent *)
{
    if (mLabel)
        mLabel->setPixmap(mPixmaps.normal);
}

void RadioKnob::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = true;
        mPressedPos = event->pos();
        mCenter = QRect({0, 0}, size()).center();
        updateLabelGeometry();
    }
}

void RadioKnob::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = false;
        updateLabelGeometry();
        emit clicked();
        if (event->pos().x() < mCenter.x())
            emit leftClicked();
        else
            emit rightClicked();
    }
}

void RadioKnob::mouseMoveEvent(QMouseEvent *)
{

}

void RadioKnob::resizeEvent(QResizeEvent *)
{
    mCenter = QRect({0, 0}, size()).center();
    updateLabelGeometry();
}

void RadioKnob::updateLabelGeometry()
{
    QWidget * btn = mLabel;
    if (btn) {
        int n = qMin(width(), height());
        int btnSize = qMin(int(MAX_BUTTON_SIZE), int(n * 0.27f + 0.5f));
        int x = int((width() - btnSize) / 2.0f + 0.5f);
        int y = int((height() - btnSize) / 2.0f + 0.5f);
//        x += x % 2;
//        y += y % 2;
        if (mPressed) {
            if (mPressedPos.x() < mCenter.x())
                --x;
            else
                ++x;
        }
        btn->setGeometry(x, y, btnSize, btnSize);
        qDebug() <<  "updateLabelGeometry" << btnSize << btnSize;
    }
}

