#include "../include/SpawnerListModel.h"

#include <QColor>
#include <QFont>


int SpawnerListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 10;
}

QVariant SpawnerListModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(index.row() * 100);

    if (role == Qt::BackgroundRole)
        return (index.row() == 0) ? QVariant(QColor(Qt::red)) : QVariant();

    if (role == Qt::ForegroundRole)
        return (index.row() % 3) == 0 ? QVariant(QColor(Qt::red).lighter()) : QVariant();

    if (role == Qt::TextAlignmentRole)
        return (index.row() > 4) ? QVariant(Qt::AlignCenter) : QVariant();

    if (role == Qt::FontRole) {
        if (index.row() > 6) {
            QFont font;
            font.setPointSize(font.pointSize() * 2);
            return font;
        }
    }

    return {};
}

Qt::ItemFlags SpawnerListModel::flags(const QModelIndex& index) const
{
    if (index.row() < 3)
        return QAbstractListModel::flags(index) & ~Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index);
}