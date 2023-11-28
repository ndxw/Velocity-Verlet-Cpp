#ifndef SPAWNERLISTMODEL_H
#define SPAWNERLISTMODEL_H

#include <QAbstractListModel>

class SpawnerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    using QAbstractListModel::QAbstractListModel;
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif