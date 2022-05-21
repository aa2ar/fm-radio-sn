/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   radioknob.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef RADIOKNOB_H
#define RADIOKNOB_H

#include "tunerknob.h"

class RadioKnob : public TunerKnob
{
    Q_OBJECT
public:
    explicit RadioKnob(QWidget * parent = 0);

    void initLabel(QPixmap, QPixmap);

signals:
    void leftClicked();
    void rightClicked();

public slots:
    void enableLabel(bool);

protected:
    void enterEvent(QEvent *)             override;
    void leaveEvent(QEvent *)             override;
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *)    override;
    void resizeEvent(QResizeEvent *)      override;

private:
    enum { MAX_BUTTON_SIZE = 24 };

    QLabel * mLabel   = nullptr;
    bool     mPressed = false;
    QPoint   mPressedPos;
    QPoint   mCenter;

    struct Pixmaps {
        QPixmap normal;
        QPixmap hovered;
    } mPixmaps;

    void updateLabelGeometry();
};

#endif // RADIOKNOB_H
