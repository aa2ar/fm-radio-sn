/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   volumeknob.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef VOLUMEKNOB_H
#define VOLUMEKNOB_H

#include "tunerknob.h"

class VolumeKnob : public TunerKnob
{
    Q_OBJECT
public:
    explicit VolumeKnob(QWidget * parent = 0);

    void initLabel(QPixmap, QPixmap);

public slots:
    void enableLabel(bool);
    void setMuted(bool);

protected:
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *)      override;

private:
    enum { MAX_BUTTON_SIZE = 24 };

    QLabel * mLabel   = nullptr;
    bool     mPressed = false;

    struct Pixmaps {
        QPixmap normal;
        QPixmap muted;
    } mPixmaps;

    void updateLabelGeometry();
};

#endif // VOLUMEKNOB_H
