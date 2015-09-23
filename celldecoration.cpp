#include "celldecoration.h"

#include <QPainter>

void CellDecoration::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  CellData *cell = static_cast<CellData*>(index.internalPointer());
  if (!cell) { return ; }

  painter->setPen(option.palette.text().color());
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.highlight());
    painter->setPen(option.palette.highlightedText().color());
  }

  if (cell->isOnlyValue()) {
    painter->setFont(QFont("arial",12,QFont::Bold));
    painter->drawText(option.rect,Qt::AlignCenter,QString::number(cell->onlyValue()));
    return;
  }

  int sssize = cell->matrix()->subSquareSize();
  if (sssize>0) {
    int stepw = cell->matrix()->cellWidth / sssize;
    int steph = cell->matrix()->cellHeight / sssize;
    int left = (option.rect.width() - cell->matrix()->cellWidth) /2 + option.rect.left()+2;
    int top = (option.rect.height() - cell->matrix()->cellHeight) /2 + option.rect.top()+steph-3;
    painter->setFont(QFont("arial",6));
    for(int i = cell->matrix()->sideSize()-1; i>=0; --i) {
      if (cell->testValue(i+1))
        painter->drawText((i%sssize)*stepw+left,(i/sssize)*steph+top,QString::number(i+1));
    }
  }
}

QSize CellDecoration::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  CellData *cell = static_cast<CellData*>(index.internalPointer());
  if (!cell) { qDebug()<<"no"; return QSize(); }
  qDebug()<<"yes";
  return QSize(cell->matrix()->cellWidth,cell->matrix()->cellHeight);
}

