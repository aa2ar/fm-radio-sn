/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   tunerknob.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef MYDIAL_H
#define MYDIAL_H

#include <QDial>

class QWheelEvent;
class QResizeEvent;
//class QToolButton;
class QLabel;
class TunerKnob : public QDial
{
    Q_OBJECT
public:
    explicit TunerKnob(QWidget * parent = 0);

signals:
    void rotatedClockwise();
    void rotatedCounterClockwise();
    void clicked();

public slots:
    void increment(int delta = 1);
    void decrement(int delta = 1);

protected:
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *)    override;
    void wheelEvent(QWheelEvent *)        override;

protected:
    bool mPressed = false;
};

#endif // MYDIAL_H
