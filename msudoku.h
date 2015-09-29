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
  bool setData(const QModelIndex &index, const QVariant &value, int role);
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
  MSudoku(int sideSize,int subSquareSize);
  const int pSubSquareSize;
  const int pSideSize;
  const int pMatrixSize;
  QVector<MCellData *> pCells;
  MSudokuValidator *cellValid;
  bool hasPosiotion(int x, int y)const{return x>=0 && x<pSideSize && y>=0 && y<pSideSize;}
  bool hasIndex(const QModelIndex &index)const{return hasPosiotion(index.column(),index.row());}
  static int getCellSize(int maxval, int squareSize, Qt::Orientation o);
};

/*
class MSudokuCell : public MCellData
{
public:
  const int x;
  const int y;
  MSudoku * model()const {return pmodel;}
  MSudokuCell(int x_,int y_,MSudoku &model):MCellData(model.sideSize()),x(x_),y(y_),pmodel(&model){}
private:
  MSudoku * pmodel;
};
*/

#endif // MSUDOKU_H
