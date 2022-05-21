/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   volumeknob.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "volumeknob.h"
//#include <QToolButton>
#include <QLabel>
#include <QWheelEvent>
#include <QIcon>
#include <QDebug>

VolumeKnob::VolumeKnob(QWidget * parent)
    : TunerKnob(parent)
{}

void VolumeKnob::initLabel(QPixmap icon, QPixmap muted)
{
    mPixmaps.normal = icon;
    mPixmaps.muted  = muted;

    if (mLabel == nullptr) {
        mLabel = new QLabel(this);
        mLabel->setAlignment(Qt::AlignCenter);
    }
    mLabel->setPixmap(icon);
    mLabel->setText(QString());
    updateLabelGeometry();
}

void VolumeKnob::enableLabel(bool value)
{
    if (mLabel)
        mLabel->setEnabled(value);
}

void VolumeKnob::setMuted(bool value)
{
    if (mLabel)
        mLabel->setPixmap(value ? mPixmaps.muted : mPixmaps.normal);
}

void VolumeKnob::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = true;
        updateLabelGeometry();
    }
}

void VolumeKnob::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = false;
        updateLabelGeometry();
        emit clicked();
    }
}

void VolumeKnob::resizeEvent(QResizeEvent *)
{
    updateLabelGeometry();
}

void VolumeKnob::updateLabelGeometry()
{
    QWidget * btn = mLabel;
    if (btn) {
//        QSize sz = btn->sizeHint();
//        float r = sz.width() / (float)sz.height();
        int n = qMin(width(), height());
        int btnSize = qMin(int(MAX_BUTTON_SIZE), int(n * 0.27f + 0.5f));
        int x = (width() - btnSize) / 2;
        int y = (height() - btnSize) / 2;
        if (mPressed) { ++x; }
        btn->setGeometry(x, y, btnSize, btnSize);
        qDebug() <<  "updateLabelGeometry" << btnSize << btnSize;
    }
}

