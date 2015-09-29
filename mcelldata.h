#ifndef MCELLDATA_H
#define MCELLDATA_H

#include <QList>
#include <QVariant>
#include <QVector>

//! хранилище всех вариантов
/*! класс представляет битовое хранилище множества значений из диапазона от 1 до size*/
class MCellData
{
public:
  MCellData(int size=0);
  void resetToAll();        //!< сбрасывает до всех возможных вариантов
  void clearToNone();       //!< устанавливает отсутсвие вариантов
  void setValue(int value); //!< устанавливает фиксированное значение
                            //!  переключает указанных вариант
  void switchValue(int value);
                            //!  возвращает флаг определенности значения
  bool isOnlyValue() const {return onlyval!=0;}
                            //!  возвращает значение которое определено или 0
  int  onlyValue() const   {return onlyval;}
                            //!  проверяет на наличие варианта
  bool testValue(int value) const;
  QList<int>values()const;  //!< возвращает список активных вариантов (медленный метод)
                            //! производит копирование данных
  MCellData(const MCellData &);
                            //! производит копирование данных
  MCellData &operator = (const MCellData & cell);
  // операции конвертации QVariant
  MCellData(const QVariant &d);
  MCellData &operator =(const QVariant &d)
     { return (*this = (d.canConvert<MCellData>())?d.value<MCellData>():MCellData());}
  operator QVariant()const {return QVariant::fromValue<MCellData>(*this);}
  enum {IntSize=sizeof(int)*8};
  const int max;      //!< количество вариантов
private:
  const int valSize;  //!< размер массива (max/IntSize+1)
  int onlyval;        //!< единственное значение
  QVector<unsigned int> val;  //!< все варианты
  int testToOnly()const;  //!< проверка на единственность значения
  //friend QDebug operator <<(QDebug,const MCellData&);
};

Q_DECLARE_METATYPE(MCellData)

#endif // MCELLDATA_H
