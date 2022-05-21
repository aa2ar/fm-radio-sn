/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

#ifndef SLIDINGTEXT_H
#define SLIDINGTEXT_H

#include <QLabel>
#include <QTimer>

class QPaintEvent;
class SlidingText : public QLabel
{
public:
    explicit SlidingText(QWidget * parent = 0);

    void enableSliding(bool);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    enum { OffsetInc = 1 };

    bool mSlidingEnabled = true;
    QTimer mTimer;
    int mTextOffset = 0;
    int mSlidingDirection = OffsetInc;

    void resetTextPos();
};

#endif // SLIDINGTEXT_H
