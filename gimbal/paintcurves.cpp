#include "paintcurves.h"

paintcurves::paintcurves(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant paintcurves::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex paintcurves::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex paintcurves::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int paintcurves::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int paintcurves::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant paintcurves::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
