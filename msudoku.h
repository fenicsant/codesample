#ifndef MSUDOKU_H
#define MSUDOKU_H

#include <QAbstractItemModel>
#include <QVector>
#include "mcelldata.h"

class MSudoku;
class MSudokuValidator;

class MSudoku : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit MSudoku(QObject *parent = 0);
              //! создание с разбиением на малые квадраты
  static MSudoku * createBySubSquareSize(int subSquareSize)
      {return new MSudoku(subSquareSize*subSquareSize,subSquareSize);}
              //! создание модели без разбиение на малые квадраты
  static MSudoku * createWithoutSubSquare(int sideSize)
      {return new MSudoku(sideSize,0);}
              //! размер стороны малого квадрата
  int subSquareSize() const {return pSubSquareSize;}
              //! размер стороны матрицы
  int sideSize() const      {return pSideSize;}
              //! количество ячеек матрицы
  int matrixSize() const    {return pMatrixSize;}
              //! возвращает ячейку матрицы по ее координатам

  MSudokuValidator & cellValidator()const {return *cellValid;}
  void setCellValidator(MSudokuValidator * validator);

  // методы необходимы для работы модели
              //! возвращает данные по их координатам и роли
  QVariant data(const QModelIndex &index, int role) const;
              //! устанавливает значение модели
  bool setData(const QModelIndex &index, const QVariant &value, int role);
              //! возвращает флаг редактируемости
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
  const MCellData & cellValue(const QModelIndex & index) const {return values[posByIndex(index)];}
private:
              //! конструктор скрыт, для корректной задачи параметров @sa createBySubSquareSize, createWithoutSubSquare
  MSudoku(int sideSize,int subSquareSize);
  enum AddRoles {
    DisplayRole     = Qt::DisplayRole,
    EditRole        = Qt::EditRole,
    BadValues       = Qt::UserRole,
    RecomendValues  = Qt::UserRole+1
  };
  const int pSubSquareSize;
  const int pSideSize;
  const int pMatrixSize;
  QVector<MCellData> values;
  QVector<MCellData> badvals;
  QVector<MCellData> recomvals;
  MSudokuValidator *cellValid;
              //! проверка корректности адреса
  bool hasPosiotion(int x, int y)const{return x>=0 && x<pSideSize && y>=0 && y<pSideSize;}
              //! проверка корректности индекса
  bool hasIndex(const QModelIndex &index)const{return hasPosiotion(index.column(),index.row());}
              //! пребразует позицию в массиве в индекс модели
  QModelIndex indexByPos(int pos)const {return createIndex(pos/pSideSize,pos%pSideSize);}
              //! преобразует индекс модели на позицию в массиве
  int posByIndex(const QModelIndex &index)const {return index.row()*pSideSize+index.column();}
};

//! подсказки решения судоку
/*! класс делит варианты значений на три вида: неопределенный, некорректный и предпочтительный */
class MSudokuValidator
{
public:
  enum Validation {
    Vague,      //!< неопределенное значение
    Brocked,    //!< некорректное значение
    Recomend    //!< предпочтительное значение
  };
  enum Relations {
    Horizontal  = 1,
    Vertical    = 2,
    SubSquare   = 4,
    All         = 7
  };

  MSudoku & owner;
        //! возвращает список связанных ячеек
  virtual QList<QModelIndex> getRelatedCells(const QModelIndex &index,Relations relation=All);
        //! возвращает true если требуется перерисовка связанных значений
  virtual bool valueChanged(const QModelIndex &index, const MCellData &prevValue);
        // ! возвращает характер значения ячейки
  //virtual Validation checkValue(CellData &cell, int value);
  MSudokuValidator(MSudoku &model) : owner(model){}
  void mcheck();
};

#endif // MSUDOKU_H
