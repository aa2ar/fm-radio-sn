/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appcontainer.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "appcontainer.h"
#include "../controlpanel.h"
#include <QResizeEvent>

AppContainer::AppContainer(QWidget *parent)
    : QWidget(parent)
{

}

void AppContainer::setBar(QWidget * bar)
{
    mBar = bar;
    mBar->setParent(this);
    resize(sizeHint());
}

void AppContainer::setControlPanel(ControlPanel * cp)
{
    Q_ASSERT(cp);
    mControlPanel = cp;
    mControlPanel->setParent(this);
    connect(mControlPanel, SIGNAL(action(int)), this, SLOT(onControlPanelAction(int)));
    resize(sizeHint());
}

void AppContainer::setPlayList(QWidget * pls)
{
    mPlayList = pls;
    mPlayList->setParent(this);
    mPlayList->setVisible(mPlayListVisible);
    resize(sizeHint());
}

// ignore mouse events
void AppContainer::mousePressEvent  (QMouseEvent * e) { e->ignore(); }
void AppContainer::mouseReleaseEvent(QMouseEvent * e) { e->ignore(); }
void AppContainer::mouseMoveEvent   (QMouseEvent * e) { e->ignore(); }

void AppContainer::resizeEvent(QResizeEvent * e)
{
    int y = 0;

    if (mControlPanel) {
        QSize szDisplay = mControlPanel->sizeHint();

        if (mBar) {
            QSize szBar = mBar->sizeHint();
            mBar->setGeometry(0, 0, szDisplay.width(), szBar.height());
            y += szBar.height();
        }

        mControlPanel->setGeometry(0, y, szDisplay.width(), szDisplay.height());
        y += szDisplay.height();

        if (mPlayList) {
            mPlayList->setGeometry(0, y, szDisplay.width(), PlayListHeight);
        }
    }
    else if (mBar) {
        QSize sz = mBar->sizeHint();
        mBar->setGeometry(0, 0, e->size().width(), sz.height());
    }
}

void AppContainer::onControlPanelAction(int a)
{
    if (a == ControlPanel::Playlist) {
        if (mPlayList) {
            mPlayListVisible = !mPlayListVisible;
            mPlayList->setVisible(mPlayListVisible);
            resize(sizeHint());
            emit layoutChanged();
        }
    }
}

QSize AppContainer::sizeHint() const
{
    QSize sz{400, 0};
    if (mControlPanel) sz = mControlPanel->sizeHint();
    if (mBar) sz.setHeight(sz.height() + mBar->sizeHint().height());
    if (mPlayList && mPlayListVisible) sz.setHeight(sz.height() + PlayListHeight);
    if (sz.width() == 0) sz = {400, 130};
    return sz;
}

