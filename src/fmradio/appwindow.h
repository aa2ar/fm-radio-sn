/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appwindow.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QFrame>
#include <QTimer>

namespace fmradio
{
    class controller_t;
}

class QEvent;
class QMouseEvent;
class QMenu;
class QListView;
class UserAgent;
class AppContainer;
class AppManager;
class ControlPanel;
class SettingsDialog;
class PlayListModel;
class PlayListProxyModel;
class AppWindow
        : public QFrame
{
    Q_OBJECT
public:
    explicit AppWindow(UserAgent *, QWidget *parent = 0);
    ~AppWindow();

signals:

public slots:

protected:
    void enterEvent(QEvent *)             override;
    void leaveEvent(QEvent *)             override;
    void mousePressEvent(QMouseEvent *)   override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *)    override;

    void contextMenuEvent(QContextMenuEvent *) override;

private:
    // ui
    enum { StickyMargin = 20, WindowMinSize = 3, BorderWidth = 3 };
    enum { FoldingDelay = 500, UnfoldingDelay = 100,
           UnfoldingAfterUnlockDelay = 1000, FoldingTimerPeriod = 10 };
    struct Dragging {
        bool   mouseDown = false;
        bool   active    = false;
        QPoint pos;
        QPoint windowPos;
    } mDragging;
    QTimer mTimer;
    bool mCanFold = true;

    enum State {
        FOLDED,
        UNFOLDING,
        FOLDING,
        UNFOLDED
    } mState = FOLDED;

    int mWidth  = WindowMinSize;
    int mHeight = WindowMinSize;
    int mX = 0;
    int mY = 0;
    int mMaxWidth  = 400;
    int mMaxHeight = 200;

    struct Borders {
        int left   = 0;
        int top    = 0;
        int right  = 0;
        int bottom = 0;
    };
    Borders mScreenBorders;
    Borders mStickyBorders;

    struct Folding {
        enum Orientation {
            HORIZONTAL = 0,
            VERTICAL   = 1,
            NONE
        };
        enum Direction {
            LEFT  = (1 << 0),
            UP    = (1 << 1),
            RIGHT = (1 << 2),
            DOWN  = (1 << 3),
            DISABLED = 0
        } direction = DISABLED;
        uint32_t available = 0;
        bool enabled = true;
        int  lockCounter = 0;
        bool isAvailable(Direction) const;
        bool isLocked() const;
        void lock();
        void unlock();
    } mFolding;

    const Qt::KeyboardModifier mDontSnapModifier = Qt::ShiftModifier;

    UserAgent      * mUserAgent = nullptr;
    AppContainer   * mContainer = nullptr;
    ControlPanel   * mControlPanel   = nullptr;

    QMenu * mMenu = nullptr;

    struct Actions {
        QAction * playlist = nullptr;
        QAction * settings = nullptr;
        QAction * close = nullptr;
    } mActions;

    PlayListModel      * mPlayListModel      = nullptr;
    PlayListProxyModel * mPlayListProxyModel = nullptr;

    SettingsDialog * mSettingsDialog = nullptr;

    // core
    AppManager * mManager = nullptr;

    void lockFolding();
    void unlockFolding();

    void setupContextMenu();
    void onContextMenuShow();
    void onContextMenuHide();

    void setupSettingsDialog();
    void onSettingsDialogShow();
    void onSettingsDialogHide();

    void tryToUnfold(int);
    void tryToFold(int);

    void onTimeout();
    QPoint snapToScreenBorders(QPoint, QSize);
    bool isSticked() const;
    void updateScreenBorders();
    void checkFoldingAvailable();
    void updateFoldingDirection();
    Folding::Orientation getFoldingOrientation();
    void setSize(QSize);
    void setState(State);

    void onControlPanelAction(int);

    void onLayoutChanged();

    QListView * setupPlaylist();
    void onPlayListItemActivated(const QModelIndex &);

    // menu actions
    void onActionPlaylist();
    void onActionSettings();
    void onActionClose();

    // debug helpers
    static QString foldingDirectionToString(Folding::Direction);
    static QString stateToString(State);
};

#endif // APPWINDOW_H
