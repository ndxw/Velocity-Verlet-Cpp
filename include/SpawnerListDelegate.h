#ifndef SPAWNERLISTDELEGATE_H
#define SPAWNERLISTDELEGATE_H

#include <QtGui/qpainter.h>

#include <QtWidgets/qitemdelegate.h>

class SpawnerListDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	using QItemDelegate::QItemDelegate;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, 
				const QModelIndex& index) const override;
};

#endif
