/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistmodel.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>

class PlayListModel : public QAbstractTableModel
{
public:
    explicit PlayListModel(QObject * parent = 0);
    ~PlayListModel();

    bool load(const QString & filename);
    bool save();

    bool insertItem(int, const QString &, bool used = true, bool fav = false, bool doSave = true);
    bool removeItem(int);
    bool removeItem(QModelIndex);

    QAbstractTableModel * viewerModel() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void clicked(QModelIndex);


    QString nameByFrequency(int);

private:
    friend class ViewerModel;

    enum { ColumnCount = 4 };
    struct Item {
        bool used;
        bool favourite;
        int  frequency;//QString???
        QString name;
        int hash;
    };

    QVector<Item *> mItems;
    QVector<Item *> mSortedItems;

    class ViewerModel;
    ViewerModel * mViewerModel = nullptr;

    QString mPlayListFile;


    inline bool indexValid(const QModelIndex &) const;
    int indexOf(int);

    const Item * itemOf(int);


    void insertAt(int, Item *);
    void removeAt(int);
    void updateViewerModel();
    static int itemHash(const Item *);
};

#endif // PLAYLISTMODEL_H
