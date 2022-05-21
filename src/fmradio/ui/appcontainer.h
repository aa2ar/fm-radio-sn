/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appcontainer.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef APPCONTAINER_H
#define APPCONTAINER_H

#include <QWidget>

class ControlPanel;
class AppContainer : public QWidget
{
    Q_OBJECT
public:
    explicit AppContainer(QWidget *parent = 0);

    void setBar(QWidget *);
    void setControlPanel(ControlPanel *);
    void setPlayList(QWidget *);

    QSize sizeHint() const override;

signals:
    void layoutChanged();

public slots:

protected:
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *)    override;
    void resizeEvent(QResizeEvent *)      override;

private slots:
    void onControlPanelAction(int);

private:
    QWidget      * mBar     = nullptr;
    ControlPanel * mControlPanel = nullptr;
    QWidget      * mPlayList= nullptr;

    enum { PlayListHeight = 150 };

    bool mPlayListVisible = false;
};

#endif // APPCONTAINER_H
