#include "msudoku.h"

QVariant MSudoku::data(const QModelIndex &index, int role) const
{
  switch (role) {
  case DisplayRole:
  case EditRole: return (hasIndex(index))?(QVariant)values[posByIndex(index)]:QVariant();
  case BadValues: return (hasIndex(index))?(QVariant)badvals[posByIndex(index)]:QVariant();

  }
  return QVariant();
}

bool MSudoku::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!hasIndex(index)) return false;
  switch (role) {
  case DisplayRole:
  case EditRole: {
    if (!value.canConvert<MCellData>()) return false;
    MCellData old = values[posByIndex(index)];
    values[posByIndex(index)] = (MCellData)value;
    if (cellValid) {
      cellValid->valueChanged(index,old);
    }
    return true;
  }
  case BadValues: {
    if (!value.canConvert<MCellData>()) return false;
    badvals[posByIndex(index)] = value;
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
  pSubSquareSize(subSquareSize),pSideSize(sideSize),
  pMatrixSize(sideSize*sideSize),values(pMatrixSize),badvals(pMatrixSize),
  recomvals(pMatrixSize),cellValid(new MSudokuValidator(*this))
{
  for(int i=0; i<pMatrixSize; ++i) {
    values[i] = MCellData(pSideSize);
    values[i].resetToAll();
    badvals[i] = MCellData(pSideSize);
    recomvals[i] = MCellData(pSideSize);
  }
}

#include <QDebug>
QList<QModelIndex> MSudokuValidator::getRelatedCells(const QModelIndex &index, Relations relation)
{
  QList<QModelIndex> res;
  const int &x = index.column();
  const int &y = index.row();
  bool subsquare = owner.subSquareSize()>0;
  for(int i=owner.sideSize()-1; i>=0; --i) {
    if ((relation&Horizontal) && i!=x) res.append(owner.index(y,i));
    if ((relation&Vertical) && i!=y) res.append(owner.index(i,x));
    if (subsquare) {
      int x2 = i%owner.subSquareSize()+(x/owner.subSquareSize())*owner.subSquareSize();
      int y2 = i/owner.subSquareSize()+(y/owner.subSquareSize())*owner.subSquareSize();
      if (relation == All && x2!=x && y2!=y) res.append(owner.index(y2,x2));
      if (relation == SubSquare && (x2!=x || y2!=y)) res.append(owner.index(y2,x2));
    }
  }
  if (res.contains(index)) {
    qDebug()<<"Error"<<relation<<index;
    res.removeAll(index);
  }
  return res;
}

bool MSudokuValidator::valueChanged(const QModelIndex &index, const MCellData &prevValue)
{
  static int level=0;
  ++level;
  QList<QModelIndex> rel;
  if (prevValue.isOnlyValue()) {
    rel = getRelatedCells(index);
    int val = prevValue.onlyValue();
    foreach(QModelIndex index,rel) {
      MCellData cell = owner.cellValue(index);
      if (!cell.testValue(val)) {
        cell.switchValue(val);
        owner.setData(index,cell,Qt::DisplayRole);
      }
    }
  }
  MCellData newVal = owner.cellValue(index);
  if (newVal.isOnlyValue()) {
    if (rel.isEmpty()) rel = getRelatedCells(index);
    int val = newVal.onlyValue();
    foreach(QModelIndex index,rel) {
      MCellData cell = owner.cellValue(index);
      if (cell.testValue(val)) {
        cell.switchValue(val);
        owner.setData(index,cell,Qt::DisplayRole);
      }
    }
  }
  if (level==1)mcheck();
  --level;
  return false;
}

void MSudokuValidator::mcheck()
{
  static bool onCheck = false;
  if (onCheck) return ;
  onCheck = true;
  static const QList<Relations> list = QList<Relations>()<<Horizontal<<Vertical<<SubSquare;

  for(int matrixIndex=owner.matrixSize()-1; matrixIndex>=0; --matrixIndex){
    QModelIndex index = owner.index(matrixIndex/owner.sideSize(),matrixIndex%owner.sideSize());
    if (owner.cellValue(index).isOnlyValue()) continue;
    foreach(Relations relation,list) {
      QList<QModelIndex> rel = getRelatedCells(index,relation);
      QList<int> values = owner.cellValue(index).values();
      for(int valueIndex=values.size()-1;valueIndex>=0;--valueIndex) {
        int &value=values[valueIndex];
        bool f = true;
        for(int relpos=rel.size()-1; relpos>=0 && f; --relpos)
          f=!owner.cellValue(rel[relpos]).testValue(value);
        if (f) {
          MCellData c = owner.cellValue(index);
          c.setValue(value);
          owner.setData(index,c,Qt::EditRole);
          onCheck = false;
          return;
        }
      }
    }
  }
  onCheck = false;
}
