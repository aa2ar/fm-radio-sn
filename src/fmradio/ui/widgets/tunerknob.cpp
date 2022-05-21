/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   tunerknob.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "tunerknob.h"
//#include <QToolButton>
#include <QLabel>
#include <QWheelEvent>
#include <QIcon>
#include <QDebug>

TunerKnob::TunerKnob(QWidget * parent)
    : QDial(parent)
{}

void TunerKnob::increment(int delta)
{
    int val = value();
    setValue(val + delta);
}

void TunerKnob::decrement(int delta)
{
    int val = value();
    setValue(val - delta);
}

void TunerKnob::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = true;
    }
}

void TunerKnob::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        mPressed = false;
        emit clicked();
    }
}

void TunerKnob::mouseMoveEvent(QMouseEvent *) {}

void TunerKnob::wheelEvent(QWheelEvent * event)
{
    if (wrapping()) {
        if (event->delta() > 0)
            emit rotatedClockwise();
        else if (event->delta() < 0)
            emit rotatedCounterClockwise();
    }
    else {
        int pos = sliderPosition();
        if (event->delta() > 0 && pos < maximum())
            emit rotatedClockwise();
        else
        if (event->delta() < 0 && pos > minimum())
            emit rotatedCounterClockwise();
    }

    if (hasTracking())
        QDial::wheelEvent(event);
    else
        event->accept();
}
