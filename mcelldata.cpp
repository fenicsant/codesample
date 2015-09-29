#include "mcelldata.h"

MCellData::MCellData(int size) :
  max(size), valSize(size/IntSize+((size%IntSize==0)?0:1)), onlyval(0),val(valSize)
{
  if (valSize == 0) return;
  if (valSize!=val.size()) val.resize(valSize);
  clearToNone();
  //resetToAll();
}

void MCellData::resetToAll()
{
  for(int i=valSize-2; i>=0; --i) val[i]=~(unsigned int)0;
  if (max%IntSize == 0) val[+valSize-1]=~(unsigned int)0;
  else val[valSize-1]=((unsigned int)1<<(max%IntSize))-1;
}

void MCellData::clearToNone()
{
  for(int i=valSize-2; i>=0; --i) val[i]=0;
}

void MCellData::setValue(int value)
{
  //Q_ASSERT(value>0 && value<=max);
  onlyval = value;
  for(int i=0; i<valSize; ++i) val[i]=0;
  --value;
  val[value/IntSize] = (unsigned int)1<<(value%IntSize);
}

void MCellData::switchValue(int value)
{
  //Q_ASSERT(value>0 && value<=max);
  --value;
  val[value/IntSize] ^= (unsigned int)1<<(value%IntSize);
  onlyval = testToOnly();
}

bool MCellData::testValue(int value) const
{
  if (max == 0) return false;
  //Q_ASSERT(value>0 && value<=max);
  --value;
  return (val[value/IntSize] & ((unsigned int)1<<(value%IntSize))) != 0;
}

QList<int> MCellData::values() const
{
  if (onlyval>0) return QList<int>()<<onlyval;
  QList<int> res;
  for(int i=0; i<max; ++i) if (testValue(i)) res.append(i);
  return res;
}

MCellData::MCellData(const MCellData &d):
  max(d.max),valSize(d.valSize),onlyval(d.onlyval),val(d.val)
{
}


MCellData &MCellData::operator =(const MCellData &d)
{
  const_cast<int&>(max) = d.max;
  const_cast<int&>(valSize) = d.valSize;
  onlyval = d.onlyval;
  val=d.val;
  return *this;
}

MCellData::MCellData(const QVariant &d) :
  max(0), valSize(0), onlyval(0),val(0)
{
  *this = d;
}

int MCellData::testToOnly() const
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
