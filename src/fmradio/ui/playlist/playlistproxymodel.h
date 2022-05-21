/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistproxymodel.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef PLAYLISTPROXYMODEL_H
#define PLAYLISTPROXYMODEL_H

#include <QAbstractListModel>

class QAbstractTableModel;
class PlayListProxyModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PlayListProxyModel(QObject *parent = 0);

    void setModel(QAbstractTableModel *);

    int columnCount(const QModelIndex &) const override { return 1; }
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:

public slots:

private:
    QAbstractTableModel * mSourceModel = nullptr;

    void onModelDataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &);
    void onModelLayoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(),
                              QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);

};

#endif // PLAYLISTPROXYMODEL_H
