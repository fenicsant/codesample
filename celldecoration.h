#ifndef CELLDECORATION_H
#define CELLDECORATION_H

#include <QStyledItemDelegate>
#include "cell.h"

class CellDecoration : public QStyledItemDelegate
{
public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // CELLDECORATION_H
