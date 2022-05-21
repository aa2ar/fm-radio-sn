/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appwindow.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "appwindow.h"
#include "appmanager.h"
#include "useragent.h"
#include "ui/appcontainer.h"
#include "ui/settings/settingsdialog.h"
#include "ui/playlist/playlistmodel.h"
#include "ui/playlist/playlistproxymodel.h"
#include "ui/playlist/playlistproxymodeldelegate.h"

#include "fmradio/controlpanel.h"

#include <QMenu>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QLabel>
#include <QListView>
//#include <QTableView>
#include <QHeaderView>
#include <QModelIndex>

// debug helpers
QString AppWindow::foldingDirectionToString(AppWindow::Folding::Direction ud)
{
    switch (ud) {
    case Folding::DISABLED:
        return QStringLiteral("Unfolding::DISABLED");
    case Folding::UP:
        return QStringLiteral("Unfolding::UP");
    case Folding::DOWN:
        return QStringLiteral("Unfolding::DOWN");
    case Folding::LEFT:
        return QStringLiteral("Unfolding::LEFT");
    case Folding::RIGHT:
        return QStringLiteral("Unfolding::RIGHT");
    }
    return QStringLiteral("???");
}

QString AppWindow::stateToString(AppWindow::State s)
{
    switch (s) {
    case State::FOLDED:
        return QStringLiteral("State::FOLDED");
    case State::UNFOLDED:
        return QStringLiteral("State::UNFOLDED");
    case State::FOLDING:
        return QStringLiteral("State::FOLDING");
    case State::UNFOLDING:
        return QStringLiteral("State::UNFOLDING");
    }
    return QStringLiteral("???");
}

AppWindow::AppWindow(UserAgent * ua, QWidget * parent)
    : QFrame(parent)
    , mUserAgent(ua)
{
    setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::BypassWindowManagerHint);

    //setAttribute(Qt::WA_TransparentForMouseEvents);
    //setWindowOpacity(0.5);

    //setFrameShape(QFrame::StyledPanel);
    //setFrameShape(NoFrame);
    setFrameShape(Panel);
    setFrameShadow(Raised);
    //setMouseTracking(true);

    updateScreenBorders();

    checkFoldingAvailable();

    setupContextMenu();

    // setup timer
    mTimer.setSingleShot(true);
    connect(&mTimer, &QTimer::timeout, this, &AppWindow::onTimeout);

    // setup container
    mContainer = new AppContainer(this);
    connect(mContainer, &AppContainer::layoutChanged, this, &AppWindow::onLayoutChanged);
    mContainer->move(BorderWidth, BorderWidth);
    mContainer->hide();

    mControlPanel = mUserAgent->getControlPanelWidget(0);
    connect(mControlPanel, &ControlPanel::action, this, &AppWindow::onControlPanelAction);

    //mContainer->setBar(new AppBar(mContainer));
    mContainer->setControlPanel(mControlPanel);

    // setup playlist model
    mPlayListModel = new PlayListModel(this);
    mPlayListModel->load("playlist.json");
    mControlPanel->setPlayListModel(mPlayListModel->viewerModel());

    // setup settings dialog
    setupSettingsDialog();

    mManager = new AppManager(mSettingsDialog);
    mControlPanel->setControlModel(mManager);
    mControlPanel->setControl(mManager);

    mContainer->setPlayList(setupPlaylist());

    // set initial state
    setState(UNFOLDED);
    move(100, 0); // FIXME: debug

    mManager->openSerialPort("/dev/ttyUSB0");
}

AppWindow::~AppWindow()
{
    delete mManager;
    delete mSettingsDialog;
}

void AppWindow::setupContextMenu()
{
    mMenu = new QMenu(this);
    mMenu->setWindowFlags(mMenu->windowFlags() | Qt::BypassWindowManagerHint);//no effect

//    mActions.playlist = mMenu->addAction(QIcon(":/rc/img/menu2gray-16x16.png"), tr("Playlist"));
//    mActions.settings = mMenu->addAction(QIcon(":/rc/img/gear-16x16.png"), tr("Tools && Settings"));
//    mMenu->addSeparator();
    mActions.close = mMenu->addAction(QIcon(":/rc/img/exit2-16x16.png"), tr("Quit"));

    connect(mMenu, &QMenu::aboutToShow, this, &AppWindow::onContextMenuShow);
    connect(mMenu, &QMenu::aboutToHide, this, &AppWindow::onContextMenuHide);

//    connect(mActions.playlist, &QAction::triggered, this, &AppWindow::onActionPlaylist);
//    connect(mActions.settings, &QAction::triggered, this, &AppWindow::onActionSettings);
    connect(mActions.close,    &QAction::triggered, this, &AppWindow::onActionClose);

//    mActions.settings->setShortcut();
}

void AppWindow::onContextMenuShow()
{
    lockFolding();
}

void AppWindow::onContextMenuHide()
{
    unlockFolding();
}

void AppWindow::setupSettingsDialog()
{
    mSettingsDialog = new SettingsDialog(mPlayListModel, this);
    connect(mSettingsDialog, &SettingsDialog::aboutToShow, this, &AppWindow::onSettingsDialogShow);
    connect(mSettingsDialog, &SettingsDialog::aboutToHide, this, &AppWindow::onSettingsDialogHide);
    connect(mSettingsDialog, &SettingsDialog::serialPortChanged, this, [this](QString value){
        mManager->setSerialPort(value);
    });
    connect(mSettingsDialog, &SettingsDialog::enableLog, this, [this](bool value){
        mManager->enableLog(value);
    });
    connect(mSettingsDialog, &SettingsDialog::logOnlyUnhandled, this, [this](bool value){
        mManager->logOnlyUnhandled(value);
    });
}

void AppWindow::onSettingsDialogShow()
{
    lockFolding();
}

void AppWindow::onSettingsDialogHide()
{
    unlockFolding();
}

void AppWindow::tryToUnfold(int tmo)
{
    if (!mDragging.mouseDown && (mState == FOLDED || (mState == UNFOLDED && !mFolding.isLocked()))) {
        updateFoldingDirection(); // FIXME: should unfold anyway?
        if (mFolding.direction != Folding::DISABLED) // ???
            mTimer.start(tmo);
    }
}

void AppWindow::tryToFold(int tmo)
{
    if (!mFolding.isLocked()
        && mFolding.direction != Folding::DISABLED
        && mState == UNFOLDED) {
        qDebug() << "try to fold";
        mTimer.setSingleShot(true);
        mTimer.start(tmo);
    }
}

void AppWindow::enterEvent(QEvent *)
{
    qDebug() << "AppWindow::enterEvent" << mDragging.mouseDown << stateToString(mState);
    tryToUnfold(UnfoldingDelay);
}

void AppWindow::leaveEvent(QEvent *)
{
    qDebug() << "AppWindow::leaveEvent";
    if (!mDragging.mouseDown) {
        mTimer.stop();
        if (mFolding.direction != Folding::DISABLED) {
            if (mState == UNFOLDED) {
                if (!mFolding.isLocked()) {
                    qDebug() << "try fold";
                    mTimer.setSingleShot(true);
                    mTimer.start(FoldingDelay);
                }
            }
            else if (mState != FOLDED) {
                mState = FOLDING;
                mTimer.setSingleShot(false);
                mTimer.start(FoldingTimerPeriod);
            }
        }
    }
}

void AppWindow::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && mState == UNFOLDED) {
        mDragging.mouseDown = true;
        mDragging.pos = event->globalPos();
    }
}

void AppWindow::mouseReleaseEvent(QMouseEvent *)
{
    mDragging.mouseDown = false;
    mDragging.active = false;
    mCanFold = mFolding.direction != Folding::DISABLED; //FIXME:depricated
    updateFoldingDirection();
    qDebug() << mCanFold << pos() << geometry().topLeft();
}

void AppWindow::mouseMoveEvent(QMouseEvent * event)
{
    if (mDragging.mouseDown) {
        if (mDragging.active) {
            mDragging.windowPos = mDragging.windowPos + (event->globalPos() - mDragging.pos);
            mDragging.pos = event->globalPos();
            if (event->modifiers() & mDontSnapModifier)
                move(mDragging.windowPos);
            else
                move(snapToScreenBorders(mDragging.windowPos, size()));
            //qDebug() << pos();
        }
        else if ((event->globalPos() - mDragging.pos).manhattanLength() > 3) {
            mDragging.active = true;
            mDragging.windowPos = pos();
            updateScreenBorders();
        }
    }
}

void AppWindow::contextMenuEvent(QContextMenuEvent * event)
{
    if (mState == UNFOLDED && mMenu)
        mMenu->exec(event->globalPos());
}

void AppWindow::lockFolding()
{
    mFolding.lock();
}

void AppWindow::unlockFolding()
{
    mFolding.unlock();
    tryToFold(UnfoldingAfterUnlockDelay);
}

void AppWindow::onControlPanelAction(int id)
{
    switch (id) {
    case ControlPanel::Playlist:
        break;
    case ControlPanel::Settings:
        onActionSettings();
        break;
    }
}

void AppWindow::onLayoutChanged()
{
    if (mState == UNFOLDED)
        setState(UNFOLDED); // trick to update size
}

QListView * AppWindow::setupPlaylist()
{
    QListView * playList = new QListView(this);
    playList->setSelectionMode(QAbstractItemView::NoSelection);

    mPlayListProxyModel = new PlayListProxyModel(this);
    mPlayListProxyModel->setModel(mPlayListModel->viewerModel());
    playList->setModel(mPlayListProxyModel);

    PlayListProxyModelDelegate * d = new PlayListProxyModelDelegate(playList);
    playList->setItemDelegate(d);

    connect(playList, &QListView::clicked, this, &AppWindow::onPlayListItemActivated);


    return playList;
}

void AppWindow::onPlayListItemActivated(const QModelIndex & index)
{
    bool ok = false;
    int freq = mPlayListProxyModel->data(index, Qt::UserRole).toInt(&ok);
    if (ok)
        mManager->onSetFrequency(freq);
}

void AppWindow::updateFoldingDirection()
{
    int x = pos().x();
    int y = pos().y();
    int bottom = y + size().height();
    qDebug() << bottom;
    if (y == mScreenBorders.top && mFolding.isAvailable(Folding::UP))
        mFolding.direction = Folding::UP;
    else if (y + size().height() == mScreenBorders.bottom && mFolding.isAvailable(Folding::DOWN))
         mFolding.direction = Folding::DOWN;
    else if (x == mScreenBorders.left && mFolding.isAvailable(Folding::LEFT))
        mFolding.direction = Folding::LEFT;
    else if (x + size().width() == mScreenBorders.right && mFolding.isAvailable(Folding::RIGHT))
         mFolding.direction = Folding::RIGHT;
    else
        mFolding.direction = Folding::DISABLED;

    qDebug() << foldingDirectionToString(mFolding.direction);
}

AppWindow::Folding::Orientation AppWindow::getFoldingOrientation()
{
    if (mFolding.direction == Folding::DISABLED) return Folding::NONE;
    if (mFolding.direction == Folding::LEFT ||
            mFolding.direction == Folding::RIGHT) return Folding::HORIZONTAL;
    return Folding::VERTICAL;
}

void AppWindow::setSize(QSize szContainer)
{
    mMaxWidth  = szContainer.width()+BorderWidth*2;
    mMaxHeight = szContainer.height()+BorderWidth*2;
    resize(mMaxWidth, mHeight);
}

void AppWindow::setState(AppWindow::State state)
{
    if (state == FOLDED) {
        // FIXME: must be sticked up
        mWidth  = WindowMinSize;
        mHeight = WindowMinSize;
        setSize(mContainer->sizeHint());
        mContainer->setVisible(true);
        mState = FOLDED;
    }
    else
    if (state == UNFOLDED) {
        setSize(mContainer->sizeHint());
        mWidth  = mMaxWidth;
        mHeight = mMaxHeight;
        setSize(mContainer->sizeHint());
        mContainer->setVisible(true);
        mState = UNFOLDED;
    }
}

void AppWindow::onActionPlaylist()
{
    // TODO:
}

void AppWindow::onActionSettings()
{
    mSettingsDialog->open();
}

void AppWindow::onActionClose()
{
    qApp->exit();
}

void AppWindow::onTimeout()
{
//    qDebug() << "onTimeout()" << stateToString(mState);

    switch (mState) {
    case FOLDED:
        updateFoldingDirection();
        if (mFolding.direction != Folding::DISABLED) {
            mState = UNFOLDING;
            mX = pos().x();
            mY = pos().y();
            mTimer.setSingleShot(false);
            mTimer.start(FoldingTimerPeriod);
            mContainer->show();
        }
        break;
    case UNFOLDING: {
        if (mFolding.direction == Folding::UP || mFolding.direction == Folding::DOWN) {
            int dy = (mHeight / 5);
            if (dy < 1) dy = 1;
            mHeight += dy;
            mY -= dy;
            if (mHeight >= mMaxHeight) {
                mHeight = mMaxHeight;
                mTimer.stop();
                mState = UNFOLDED;

                if (mFolding.direction == Folding::DOWN)
                    mY = mScreenBorders.bottom - mHeight;
            }
        }
        else if (mFolding.direction == Folding::LEFT || mFolding.direction == Folding::RIGHT) {
            int dx = (mWidth / 5);
            if (dx < 1) dx = 1;
            mWidth += dx;
            mX -= dx;
            if (mWidth >= mMaxWidth) {
                mWidth = mMaxWidth;
                mTimer.stop();
                mState = UNFOLDED;

                if (mFolding.direction == Folding::RIGHT)
                    mX = mScreenBorders.right - mWidth;
            }
        }
        if (mFolding.direction == Folding::UP || mFolding.direction == Folding::LEFT)
            resize(mWidth, mHeight);
        else
            setGeometry(mX, mY, mWidth, mHeight);
    } break;
    case FOLDING: {
        if (mFolding.direction == Folding::UP || mFolding.direction == Folding::DOWN) {
            int dy = (mHeight / 5);
            if (dy < 1) dy = 1;
            mHeight -= dy;
            mY += dy;
            if (mHeight <= WindowMinSize) {
                mHeight = WindowMinSize;
                mTimer.stop();
                mState = FOLDED;
                mContainer->hide();

                if (mFolding.direction == Folding::DOWN)
                    mY = mScreenBorders.bottom - mHeight;
            }
        }
        else if (mFolding.direction == Folding::LEFT || mFolding.direction == Folding::RIGHT) {
            int dx = (mWidth / 5);
            if (dx < 1) dx = 1;
            mWidth -= dx;
            mX += dx;
            if (mWidth <= WindowMinSize) {
                mWidth = WindowMinSize;
                mTimer.stop();
                mState = FOLDED;
                mContainer->hide();

                if (mFolding.direction == Folding::RIGHT)
                    mX = mScreenBorders.right - mWidth;
            }
        }
        if (mFolding.direction == Folding::UP || mFolding.direction == Folding::LEFT)
            resize(mWidth, mHeight);
        else
            setGeometry(mX, mY, mWidth, mHeight);
    } break;
    case UNFOLDED:
        qDebug() << frameGeometry() << QCursor::pos();
        if (!frameGeometry().contains(QCursor::pos()) && !mDragging.mouseDown) {
            updateFoldingDirection();
            if (mFolding.direction != Folding::DISABLED) {
                mX = pos().x();
                mY = pos().y();
                mState = FOLDING;
                mTimer.setSingleShot(false);
                mTimer.start(FoldingTimerPeriod);
            }
        }
        break;
    }
}

QPoint AppWindow::snapToScreenBorders(QPoint pos, QSize size)
{
    int x = pos.x(), y = pos.y();
    if (x < mStickyBorders.left) x = mScreenBorders.left;
    else if (x + size.width() > mStickyBorders.right) x = mScreenBorders.right - size.width();
    if (y < mStickyBorders.top) y = mScreenBorders.top;
    else if (y + size.height() > mStickyBorders.bottom) y = mScreenBorders.bottom - size.height();
    return {x, y};
}

bool AppWindow::isSticked() const
{
    return !mCanFold;
}

void AppWindow::updateScreenBorders()
{
    //QRect screenRect = QApplication::screens()[0]->availableVirtualGeometry();
    QRect screenRect = QApplication::screens()[0]->virtualGeometry();
    mScreenBorders.left   = screenRect.left();
    mScreenBorders.top    = screenRect.top();
    mScreenBorders.right  = screenRect.right()+1;
    mScreenBorders.bottom = screenRect.bottom()+1;
    mStickyBorders.left   = mScreenBorders.left + StickyMargin;
    mStickyBorders.top    = mScreenBorders.top  + StickyMargin;
    mStickyBorders.right  = mScreenBorders.right  - StickyMargin;
    mStickyBorders.bottom = mScreenBorders.bottom - StickyMargin;
}

void AppWindow::checkFoldingAvailable()
{
    mFolding.available = 0x0F;
}


bool AppWindow::Folding::isAvailable(AppWindow::Folding::Direction dir) const
{
    return (available & dir) == dir;
}

bool AppWindow::Folding::isLocked() const
{
    return (lockCounter > 0);
}

void AppWindow::Folding::lock()
{
    ++lockCounter;
}

void AppWindow::Folding::unlock()
{
    if (lockCounter > 0) --lockCounter;
}
