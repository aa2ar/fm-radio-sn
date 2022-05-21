/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   logmodel.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>

class LogModel : public QAbstractListModel
{
    friend class LogModelDelegate;

public:
    explicit LogModel(QObject * parent = 0);

    void addInputData(const QString &);
    void addOutputData(const QString &);
    void addInfoData(const QString &);

    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    enum { ItemsMaxCount = 100, ItemsLimit = 110 };
    struct Item {
        enum Type { INPUT, OUTPUT, INFO };
        Type type;
        QStringList data;
    };
    QVector<Item> mItems;
    void checkItemCount();
    void addItem(Item::Type, const QString &);
};

#endif // LOGMODEL_H
