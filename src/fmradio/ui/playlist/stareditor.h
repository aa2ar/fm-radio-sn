/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   stareditor.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef STAREDITOR_H
#define STAREDITOR_H

#include <QWidget>

class StarEditor : public QWidget
{
    Q_OBJECT
public:
    explicit StarEditor(QWidget * parent = 0);

    void setImages(QImage, QImage);

    bool value() const;
    void setValue(bool);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

signals:
    void editingFinished();

public slots:

private:
    bool mValue = false;
    QImage mImgTrue;
    QImage mImgFalse;
};

#endif // STAREDITOR_H
