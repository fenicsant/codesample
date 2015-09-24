#include "cell.h"
#include <QString>
#include <QStringList>
#include <QHeaderView>
#include <QPainter>

QDebug operator <<(QDebug d,const CellData &cd)
{
  QStringList value;
  for(int i=cd.valSize-1; i>=0; --i) {
    QString s = QString::number(cd.val[i],16);
    s.prepend(QString().fill('0',cd.IntSize/4-s.size()));
    if (s.size()>4) s.insert(4," ");
    value.append(s);
  }
  return (d.nospace()<<"<m:"<<cd.max<<" ov:"<<cd.onlyval<<" val:"<<value.join(" ").toAscii().constData()<<">").space();
}

/* ****************  CellMatrix  **************** */

QModelIndex CellMatrix::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasPosiotion(column,row)) return QModelIndex();
  return createIndex(row,column,(void*)&cell(column,row));
}

CellMatrix::CellMatrix(int sideSize, int subSquareSize) :
  cellWidth(getCellSize(sideSize,subSquareSize,Qt::Horizontal)),
  cellHeight(getCellSize(sideSize,subSquareSize,Qt::Vertical)),
  pSubSquareSize(subSquareSize),pSideSize(sideSize),
  pMatrixSize(sideSize*sideSize),pCells(0)
{
  pCells.resize(pMatrixSize);
  for(int i=0; i<pMatrixSize; ++i) {
    pCells[i] = new CellData(*this,i%pSideSize,i/pSideSize);
  }
}

int CellMatrix::getCellSize(int maxval, int squareSize, Qt::Orientation o)
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

/* ****************  CellData  **************** */

CellData::CellData(CellMatrix &matrix, int ax, int ay) :
  x(ax),y(ay),max(matrix.sideSize()),
  valSize(matrix.sideSize()/IntSize+((matrix.sideSize()%IntSize==0)?0:1)),
  pmatrix(&matrix),onlyval(0),val(0)
{
  val = new unsigned int[valSize];
  resetToAll();
  //for(int i=0; i<valSize; ++i) val[i]=0;
}

void CellData::resetToAll()
{
  for(int i=valSize-2; i>=0; --i) val[i]=~(unsigned int)0;
  if (max%IntSize == 0) val[+valSize-1]=~(unsigned int)0;
  else val[valSize-1]=((unsigned int)1<<(max%IntSize))-1;
}

void CellData::setValue(int value)
{
  Q_ASSERT(value>0 && value<=max);
  onlyval = value;
  for(int i=0; i<valSize; ++i) val[i]=0;
  --value;
  val[value/IntSize] = (unsigned int)1<<(value%IntSize);
}

void CellData::switchValue(int value)
{
  Q_ASSERT(value>0 && value<=max);
  --value;
  val[value/IntSize] ^= (unsigned int)1<<(value%IntSize);
  onlyval = testToOnly();
}

bool CellData::testValue(int value) const
{
  Q_ASSERT(value>0 && value<=max);
  --value;
  return (val[value/IntSize] & ((unsigned int)1<<(value%IntSize))) != 0;
}

QList<int> CellData::values() const
{
  if (onlyval>0) return QList<int>()<<onlyval;
  QList<int> res;
  for(int i=0; i<max; ++i) if (testValue(i)) res.append(i);
  return res;
}

CellData::CellData(const CellData &c) :
  x(c.x),y(c.y),max(c.max),valSize(c.valSize),pmatrix(c.pmatrix),
  onlyval(c.onlyval),val(new unsigned int[c.valSize])
{
  for(int i=0; i<valSize; ++i) val[i] = c.val[i];
}

CellData::~CellData()
{
  delete[] val;
}

int CellData::testToOnly() const
{
  int hindex = -1;
  unsigned int lowval = 0;
  for(int i=0; i<valSize; ++i) {
    if (val[i]) {
      if (lowval) return 0;
      hindex = i;
      lowval = val[i];
    }
  }
  if (lowval==0) return 0;
  int lindex = 1;
  while ((lowval & 1) == 0) {lowval >>=1; ++lindex;}
  if (lowval>>1) return 0;
  return hindex*IntSize+lindex;
}




