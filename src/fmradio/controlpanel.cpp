/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   controlpanel.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{}

void ControlPanel::setControlModel(QAbstractListModel * m)
{
    if (mControlModel) {
        disconnect(mControlModel, &QAbstractListModel::dataChanged, this, &ControlPanel::onControlModelDataChanged);
    }

    mControlModel = m;
    if (mControlModel) {
        connect(mControlModel, &QAbstractListModel::dataChanged, this, &ControlPanel::onControlModelDataChanged);

        for (int i = 0; i < mControlModel->rowCount(); ++i)
            syncControlModelData(i);
    }
}

void ControlPanel::setPlayListModel(QAbstractItemModel * model)
{
    mPlayListModel = model;

    if (mPlayListModel) {
        disconnect(mPlayListModel, &QAbstractItemModel::dataChanged, this, &ControlPanel::onPlayListModelDataChanged);
        disconnect(mPlayListModel, &QAbstractItemModel::layoutChanged, this, &ControlPanel::onPlayListModelLayoutChanged);
    }

    mPlayListModel = model;
    connect(mPlayListModel, &QAbstractItemModel::dataChanged, this, &ControlPanel::onPlayListModelDataChanged);
    connect(mPlayListModel, &QAbstractItemModel::layoutChanged, this, &ControlPanel::onPlayListModelLayoutChanged);

    syncPlayListModelData();
}

void ControlPanel::setControl(UiControl * c)
{
    mControl = c;
}

void ControlPanel::onControlModelDataChanged(const QModelIndex & i1, const QModelIndex & i2, const QVector<int> &)
{
    for (int r = i1.row(); r <= i2.row(); ++r)
        syncControlModelData(r);
}

void ControlPanel::onPlayListModelDataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)
{
    syncPlayListModelData();
}

void ControlPanel::onPlayListModelLayoutChanged(const QList<QPersistentModelIndex> &parents,
                                                QAbstractItemModel::LayoutChangeHint hint)
{
    syncPlayListModelData();
}

void ControlPanel::syncControlModelData(int /*row*/)
{}
