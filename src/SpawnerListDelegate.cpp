#include "../include/SpawnerListDelegate.h"

void SpawnerListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	const auto amount = index.data().value<int>();
    painter->save();
    QStyleOptionViewItem opt = setOptions(index, option);
    drawBackground(painter, opt, index);
    drawDisplay(painter, opt, option.rect, QString::number(amount));
    drawFocus(painter, opt, option.rect);
    painter->restore();
}