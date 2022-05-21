/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   playlistmodel.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "playlistmodel.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

class PlayListModel::ViewerModel : public QAbstractTableModel
{
public:
    explicit ViewerModel(QObject * parent = 0)
        : QAbstractTableModel(parent)
    {}

    void setItems(const QVector<PlayListModel::Item *> * list) {
        mList = list;
        emit layoutChanged();
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const {
        Q_UNUSED(parent)
        return mList ? mList->size() : 0;
    }
    int columnCount(const QModelIndex &) const {
        return 4;
    }
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const
    {
        Q_UNUSED(role)
        if (mList == nullptr || index.row() < 0 || index.row() >= mList->size()) return QVariant();
        const PlayListModel::Item * item = (*mList)[index.row()];
        switch (index.column()) {
        case 0: return item->favourite;
        case 1: return item->frequency;
        case 2: return item->name;
        }
        return QVariant();
    }
    Qt::ItemFlags flags(const QModelIndex &) const
    {
        return (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    }

private:
    const QVector<PlayListModel::Item *> * mList = nullptr;
};


// PlayListModel
PlayListModel::PlayListModel(QObject * parent)
    : QAbstractTableModel(parent)
{
    mViewerModel = new ViewerModel(this);
}

PlayListModel::~PlayListModel()
{
    qDeleteAll(mItems);
}

bool PlayListModel::load(const QString & filename)
{
    mPlayListFile = filename;

    QFile loadFile(mPlayListFile);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonParseError err;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &err));

    if (err.error != QJsonParseError::NoError) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject root = loadDoc.object();

    if (root.contains("playlist")) {
        QJsonValue jValAry = root.value("playlist");
        if (!jValAry.isArray()) {
            qWarning("Couldn't open save file.");
            return false;
        }

        QJsonArray jAry = jValAry.toArray();

        for (int i = 0; i < jAry.size(); ++i) {

            QJsonValue jValItem = jAry.at(i);
            if (jValItem.isObject()) {
                QJsonObject jObjItem = jValItem.toObject();

                QJsonValue jValFreq = jObjItem.value("freq");
                QJsonValue jValName = jObjItem.value("name");
                QJsonValue jValUsed = jObjItem.value("used");
                QJsonValue jValFav  = jObjItem.value("fav");

                if (jValFreq.isDouble()
                        && jValName.isString()
                        && jValUsed.isBool()
                        && jValFav.isBool()) {

                    insertItem(int(jValFreq.toDouble(100.0)),
                               jValName.toString(),
                               jValUsed.toBool(),
                               jValFav.toBool(), false);

                }
            }
        }
    }


    return true;
}

bool PlayListModel::save()
{
    if (mPlayListFile.isEmpty())
        return false;

    QFile saveFile(mPlayListFile);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject root;
    QJsonArray  ary;

    for (int i = 0; i < mItems.size(); ++i) {
        Item * item = mItems[i];

        QJsonObject jObj;
        jObj.insert("freq", item->frequency);
        jObj.insert("name", item->name);
        jObj.insert("used", item->used);
        jObj.insert("fav",  item->favourite);

        ary.append(jObj);
    }

    root.insert("playlist", ary);

    QJsonDocument saveDoc(root);
    saveFile.write(saveDoc.toJson());

    return true;
}

bool PlayListModel::insertItem(int freq, const QString & name, bool used, bool fav, bool doSave)
{
    int idx = indexOf(freq);
    if (idx > -1)
        return false;

    Item * newItem = new Item{used, fav, freq, name, 0};

    for (int i = 0; i < mItems.size(); ++i) {
        const Item * item = mItems[i];
        if (freq < item->frequency) {
            insertAt(i, newItem);
            return true;
        }
    }

    insertAt(mItems.size(), newItem);

    if (doSave)
        save();

    return true;
}

bool PlayListModel::removeItem(int freq)
{
    int idx = indexOf(freq);
    if (idx == -1)
        return false;

    removeAt(idx);

    save();

    return true;
}

bool PlayListModel::removeItem(QModelIndex index)
{
    if (!indexValid(index))
        return false;

    removeAt(index.row());

    save();

    return true;
}

QAbstractTableModel * PlayListModel::viewerModel() const
{
    return mViewerModel;
}

int PlayListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mItems.size();
}

int PlayListModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}

QVariant PlayListModel::data(const QModelIndex & index, int role) const
{
    if (indexValid(index))
        if (role == Qt::DisplayRole) {
            const Item * item = mItems[index.row()];
            switch (index.column()) {
            case 0: // used
                return item->used;
            case 1: // favourite
                return item->favourite;
            case 2: // frequency
                return item->frequency;
            case 3: // name
                return item->name;
            }
        }

    return QVariant();
}

QVariant PlayListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: // used
            return QStringLiteral("");
        case 1: // favourite
            return QStringLiteral("");
        case 2: // frequency
            return QStringLiteral("Frequency");
        case 3: // name
            return QStringLiteral("Title");
        }
    }

    return QVariant();
}

Qt::ItemFlags PlayListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    if (index.column() == 0 || index.column() == 1 || index.column() == 3)
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool PlayListModel::setData(const QModelIndex & index, const QVariant & value, int /*role*/)
{
    if (indexValid(index)) {
        if (index.column() == 1) {
            Item * item = mItems[index.row()];
            item->favourite = value.toBool();
            return true;
        }
        if (index.column() == 3) {
            Item * item = mItems[index.row()];
            item->name = value.toString();
            updateViewerModel();
            emit dataChanged(index, index);

            save();

            return true;
        }
    }
    return false;
}

void PlayListModel::clicked(QModelIndex index)
{
    if (indexValid(index)) {
        Item * item = mItems[index.row()];
        if (index.column() == 0) {
            item->used = !item->used;
            emit dataChanged(index, index);
            updateViewerModel();
        }
        else
        if (index.column() == 1) {
            item->favourite = !item->favourite;
            emit dataChanged(index, index);
            updateViewerModel();
        }
    }
}

QString PlayListModel::nameByFrequency(int freq)
{
    const Item * item = itemOf(freq);
    return item ? item->name : QString();
}

inline bool PlayListModel::indexValid(const QModelIndex & index) const
{
    if (index.row() < 0 || index.row() >= mItems.size()) return false;
    if (index.column() < 0 || index.column() >= ColumnCount) return false;
    return true;
}

int PlayListModel::indexOf(int freq)
{
    for (int i = 0; i < mItems.size(); ++i) {
        const Item * item = mItems[i];
        if (item->frequency == freq)
            return i;
    }
    return -1;
}

const PlayListModel::Item * PlayListModel::itemOf(int freq)
{
    for (int i = 0; i < mItems.size(); ++i) {
        const Item * item = mItems[i];
        if (item->frequency == freq)
            return item;
    }
    return nullptr;
}

void PlayListModel::insertAt(int idx, PlayListModel::Item * item)
{
//    emit beginInsertRows(index(idx, 0), idx, idx);
    emit layoutAboutToBeChanged();
    mItems.insert(idx, item);
//    emit endInsertRows();
    emit layoutChanged();

    updateViewerModel();

    save();
}

void PlayListModel::removeAt(int idx)
{
//    emit beginInsertRows(index(idx, 0), idx, idx);
    emit layoutAboutToBeChanged();
    mItems.remove(idx);
//    emit endInsertRows();
    emit layoutChanged();

    updateViewerModel();
}

void PlayListModel::updateViewerModel()
{
    mSortedItems.clear();

    std::copy_if(mItems.begin(), mItems.end(),
                 std::back_inserter(mSortedItems),
                 [](const Item * i){
        return i->used;
    });

    for (auto i: mSortedItems)
        i->hash = itemHash(i);

    std::sort(mSortedItems.begin(), mSortedItems.end(),
              [](const Item * lhs, const Item * rhs){
        return lhs->hash < rhs->hash;
    });

    for (const Item * i: mSortedItems)
        qDebug() << i->hash << i->favourite << i->frequency << i->name;

    mViewerModel->setItems(&mSortedItems);
}

int PlayListModel::itemHash(const PlayListModel::Item * item)
{
    int h = item->favourite ? 0 : 10000;
    return (h + item->frequency);
}

