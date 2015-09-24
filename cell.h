#ifndef CELL_H
#define CELL_H
/*! @file cell.h
    @brief файл описывает модель данных судоку произвольного размера
 */
#include <QList>
#include <QVector>
#include <QAbstractItemModel>

#include <QDebug>

class CellData;
//! модель данных судоку
/*! Модель имеет два представления, содержащая малые квадраты, и не содержащая
  малые квадраты. В первом случае размер стороны задается как квадрат размера
  малого квадрата, в другом задается просто размер стороны. */
class CellMatrix : public QAbstractItemModel
{
public:
            //! создание с разбиением на малые квадраты
  static CellMatrix * createBySubSquareSize(int subSquareSize)
      {return new CellMatrix(subSquareSize*subSquareSize,subSquareSize);}
            //! создание модели без разбиение на малые квадраты
  static CellMatrix * createWithoutSubSquare(int sideSize)
      {return new CellMatrix(sideSize,0);}
            //! размер стороны малого квадрата
  int subSquareSize() const {return pSubSquareSize;}
            //! размер стороны матрицы
  int sideSize() const      {return pSideSize;}
            //! количество ячеек матрицы
  int matrixSize() const    {return pMatrixSize;}
            //! возвращает ячейку матрицы по ее координатам
  CellData &cell(int x,int y)
      {Q_ASSERT(hasPosiotion(x,y));return *pCells[y*pSideSize+x];}
            //! возвращает ячейку матрицы по ее координатам
  const CellData &cell(int x,int y) const
      {Q_ASSERT(hasPosiotion(x,y));return *pCells[y*pSideSize+x];}
  // методы необходимы для работы модели
            //! возвращает данные по их координатам и роли
  QVariant data(const QModelIndex &index, int role) const {return QVariant();}
  Qt::ItemFlags flags(const QModelIndex &index) const {return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled; }
  // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
            //! генерирует индекс модели по координатам и родительскому узлу
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
            //! генерирует индекс родительского узла
  QModelIndex parent(const QModelIndex &index) const {return QModelIndex();}
            //! возвращает размер матрицы по вертикали
  int rowCount(const QModelIndex &parent = QModelIndex()) const {return pSideSize;}
            //! возвращает размер матрицы по горизонтали
  int columnCount(const QModelIndex &parent = QModelIndex()) const {return pSideSize;}
            //! расчетный размер ячейки по горизонтали
  const int cellWidth;
            //! расчетный размер ячейки по вертикали
  const int cellHeight;
private:
            //! конструктор скрыт, для корректной задачи параметров @sa createBySubSquareSize, createWithoutSubSquare
  CellMatrix(int sideSize,int subSquareSize);
  const int pSubSquareSize;
  const int pSideSize;
  const int pMatrixSize;
  QVector<CellData*> pCells;
  bool hasPosiotion(int x, int y)const{return x>=0 && x<pSideSize && y>=0 && y<pSideSize;}
  static int getCellSize(int maxval, int squareSize, Qt::Orientation o);
};

//! хранилище всех вариантов
/*! класс представляет битовое хранилище множества значений из диапазона от 1 до size*/
class CellData
{
public:
  CellData(CellMatrix& matrix,int x, int y);
  void resetToAll();        //!< сбрасывает до всех возможных вариантов
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
                            //! производит копирование узла
  explicit CellData(const CellData &);
  ~CellData();              //!< деструктор

  CellMatrix *matrix() {return pmatrix;}
  const int x;
  const int y;
private:
  enum {IntSize=sizeof(int)*8};
  const int max;      //!< количество вариантов
  const int valSize;  //!< размер массива (max/IntSize+1)
  CellMatrix *pmatrix;
  int onlyval;        //!< единственное значение
  unsigned int *val;  //!< все варианты
  int testToOnly()const;  //!< проверка на единственность значения
  friend QDebug operator <<(QDebug,const CellData&);
  CellData &operator = (const CellData &){Q_ASSERT(false); return *this;}
};

#endif // CELL_H
