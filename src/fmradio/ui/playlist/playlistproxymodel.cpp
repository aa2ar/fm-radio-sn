/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistproxymodel.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "playlistproxymodel.h"

PlayListProxyModel::PlayListProxyModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void PlayListProxyModel::setModel(QAbstractTableModel * m)
{
    if (mSourceModel) {
        disconnect(mSourceModel, &QAbstractItemModel::dataChanged, this, &PlayListProxyModel::onModelDataChanged);
        disconnect(mSourceModel, &QAbstractItemModel::layoutChanged, this, &PlayListProxyModel::onModelLayoutChanged);
    }
    mSourceModel = m;
    if (mSourceModel) {
        connect(mSourceModel, &QAbstractItemModel::dataChanged, this, &PlayListProxyModel::onModelDataChanged);
        connect(mSourceModel, &QAbstractItemModel::layoutChanged, this, &PlayListProxyModel::onModelLayoutChanged);
    }
}

int PlayListProxyModel::rowCount(const QModelIndex &) const
{
    return (mSourceModel == nullptr) ? 0 : mSourceModel->rowCount();
}

QVariant PlayListProxyModel::data(const QModelIndex & index, int role) const
{
    if (mSourceModel == nullptr || role == Qt::CheckStateRole)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        float freq = (float)(mSourceModel->data(mSourceModel->index(index.row(), 1), role).toInt()) / 10.0f;
        QString s;
        s.sprintf("%.1f MHz", freq);
        QString sName = mSourceModel->data(mSourceModel->index(index.row(), 2)).toString();
        if (!sName.isEmpty()) {
            s += QStringLiteral(" - ") + sName;
        }
        return s;
    }

    if (role == Qt::UserRole)
        return mSourceModel->data(mSourceModel->index(index.row(), 1), role);

    return mSourceModel->data(index, role);
}

void PlayListProxyModel::onModelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    emit dataChanged(topLeft, bottomRight, roles);
}

void PlayListProxyModel::onModelLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    emit layoutChanged(parents, hint);
}

