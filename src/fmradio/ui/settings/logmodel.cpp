/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   logmodel.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "logmodel.h"

LogModel::LogModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void LogModel::addInputData(const QString & data)
{
    addItem(Item::INPUT, data);
}

void LogModel::addOutputData(const QString & data)
{
    addItem(Item::OUTPUT, data);
}

void LogModel::addInfoData(const QString & data)
{
    addItem(Item::INFO, data);
}

void LogModel::clear()
{
    mItems.clear();
    emit layoutChanged();
}

// QAbstractListModel
int LogModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mItems.size();
}

int LogModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant LogModel::data(const QModelIndex & /*index*/, int /*role*/) const
{
    return QVariant();
}

void LogModel::checkItemCount()
{
    if (mItems.size() > ItemsLimit) {
        mItems.remove(0, mItems.size() - ItemsMaxCount);
        emit layoutChanged();
    }
}

void LogModel::addItem(LogModel::Item::Type type, const QString & data)
{
    if (mItems.isEmpty())
        mItems.append({type, {data}});
    else {
        Item & item = mItems.last();
        if (item.type == type) {
            if (item.data.size() > 10)
                mItems.append({type, {data}});
            else
                item.data.append(data);
        }
        else
            mItems.append({type, {data}});
    }
    emit layoutChanged();
}

