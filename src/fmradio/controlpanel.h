/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   controlpanel.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QAbstractListModel>

class UiControl;
class ControlPanel
        : public QWidget
{
    Q_OBJECT
public:
    enum Action {
        Playlist = 1,
        Settings = 2,
    };

public:
    explicit ControlPanel(QWidget *parent = 0);

    void setControlModel(QAbstractListModel *);
    void setPlayListModel(QAbstractItemModel *);
    virtual void setControl(UiControl *);

signals:
    void action(int);

public slots:

private slots:
    void onControlModelDataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &);
    void onPlayListModelDataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &);
    void onPlayListModelLayoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(),
                                      QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);

protected:
    QAbstractListModel * mControlModel  = nullptr;
    QAbstractItemModel * mPlayListModel = nullptr;
    UiControl          * mControl = nullptr;

    virtual void syncControlModelData(int);
    virtual void syncPlayListModelData() {}
};

#endif // CONTROLPANEL_H
