/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   slidingtext.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "slidingtext.h"
#include <QPaintEvent>
#include <QPainter>

SlidingText::SlidingText(QWidget * parent)
    : QLabel(parent)
{
    mTimer.setInterval(25);
    connect(&mTimer, &QTimer::timeout, this, [this]{
        mTextOffset += mSlidingDirection;
        update();
    });
}

void SlidingText::enableSliding(bool enable)
{
    if (mSlidingEnabled == enable)
        return;

    mSlidingEnabled = enable;
    update();
}

void SlidingText::paintEvent(QPaintEvent * event)
{
    if (!mSlidingEnabled) {
        QLabel::paintEvent(event);
        return;
    }

    QPainter p(this);
    const QString & txt = text();
    const int textWidth = QFontMetrics(font()).width(txt);

    p.fillRect(rect(), palette().background());

    if (mTimer.isActive()) {
        if (txt.isEmpty()) {
            mTimer.stop();
            return;
        }
    }
    else {
        if (!txt.isEmpty())
            mTimer.start();
    }

    if (mSlidingDirection > 0) {
        if (mTextOffset + textWidth > width()) {
            mSlidingDirection = -OffsetInc;
            mTextOffset = width() - textWidth;
        }
    }
    else {
        if (mTextOffset < 0) {
            mSlidingDirection = OffsetInc;
            mTextOffset = 0;
        }
    }

    p.drawText(mTextOffset, 0, textWidth, height(), Qt::AlignCenter, txt);
}
