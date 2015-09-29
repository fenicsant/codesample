#include "msudoku.h"

QVariant MSudoku::data(const QModelIndex &index, int role) const
{
  if (!hasIndex(index)) return QVariant();
  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole: return *pCells[index.column()+index.row()*pSideSize];
  }
  return QVariant();
}

bool MSudoku::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!hasIndex(index)) return false;
  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole: {
    if (!value.canConvert<MCellData>()) return false;
    *pCells[index.column()+index.row()*pSideSize] = value;
    return true;
  }
  }
  return false;
}

QModelIndex MSudoku::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasPosiotion(column,row)) return QModelIndex();
  return createIndex(row,column);
}

MSudoku::MSudoku(int sideSize, int subSquareSize) :
  cellWidth(getCellSize(sideSize,subSquareSize,Qt::Horizontal)),
  cellHeight(getCellSize(sideSize,subSquareSize,Qt::Vertical)),
  pSubSquareSize(subSquareSize),pSideSize(sideSize),
  pMatrixSize(sideSize*sideSize),pCells(0)
{
  pCells.resize(pMatrixSize);
  for(int i=0; i<pMatrixSize; ++i) {
    //pCells[i] = new MSudokuCell(i%pSideSize,i/pSideSize,*this);
  }
}

#include <QFontMetrics>
#include <QStringList>
int MSudoku::getCellSize(int maxval, int squareSize, Qt::Orientation o)
{
  QFontMetrics fm(QFont("Arial",6));
  int res=0;
  if (squareSize==0) {
    QStringList list;
    for(int i=1; i<=maxval; ++i)
      list.append(QString::number(i));
    QRect r = fm.boundingRect(list.join(" "));
    res = (r.right()<r.bottom())?r.bottom():r.right();
  } else {
    for(int i=1; i<=maxval; ++i) {
      QRect r = fm.boundingRect(QString::number(i));
      if (o==Qt::Horizontal) {
        if (r.width()>res) res = r.width();
      } else {
        if (r.height()>res) res = r.height();
      }
    }
    res += 4;
    res *= squareSize;
  }
  fm = QFontMetrics(QFont("arial",12,QFont::Bold));
  for(int i=1; i<=maxval; ++i) {
    QRect r = fm.boundingRect(QString::number(i));
    if (o==Qt::Horizontal) {
      if (r.width()>res) res = r.width();
    } else {
      if (r.height()>res) res = r.height();
    }
  }
  return res;
}

