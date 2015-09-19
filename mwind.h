#ifndef MWIND_H
#define MWIND_H

#include <QWidget>
#include <QVector>
#include <QLabel>

#include<QDebug>


//! главное окно программы
class MWind : public QWidget
{
  Q_OBJECT
public:
  enum {
    SubSquare = 4,                //!< размер малого квадрата
    SideSize=SubSquare*SubSquare, //!< размер стороны, максимум определяется размерностью int-1
    MatrixSize=SideSize*SideSize  //!< количество ячеек матрицы
  };
                    //! функция отображения окна. что более?
  static void showInst() {(new MWind)->show();}
  class Cell;       //!< ячейка матрицы для отображения на экране
protected:
  //! на тот случай если окно будет не главным или их будет несколько
  void closeEvent(QCloseEvent *e) {QWidget::closeEvent(e);deleteLater();}
private:
  explicit MWind(QWidget *parent = 0);
  QVector<Cell*> cells;               //!< общий набор ячеек
};

// ячейка матрицы для отображения на экране
class MWind::Cell : public QLabel
{
  Q_OBJECT
public:
  explicit Cell(QWidget * parent, QVector<Cell*> &cells, int x, int y);
  QVector<Cell*> &cells;  //!< все ячейки
  const int x;                  //!< горизонтальная координата этой ячейки
  const int y;                  //!< вертикальная координата этой ячейки
  Cell & byCoord(int x,int y)
  {return *cells[y*SideSize+x];} //!< возвращает ячейку по ее координатам
  Cell & byCoord(int x,int y) const
  {return *cells[y*SideSize+x];} //!< возвращает ячейку по ее координатам
  // работа со значениями
  enum Digit {
    D1=1,   //!< цифра 1
    D2=2,   //!< цифра 2
    D3=4,   //!< цифра 3
    D4=8,   //!< цифра 4
    D5=0x10, //!< цифра 5
    D6=0x20, //!< цифра 6
    D7=0x40, //!< цифра 7
    D8=0x80, //!< цифра 8
    D9=0x100 //!< цифра 9
  };                                //!< набор битов допустимых цифр
  int value() const {return val;}   //!< возвращает хранилище значений
  void setAllValues() {val = 0x1FF; updateVal();} //!< сбрасывает значения на все допустимые
  void rmVal(int v) {val &= ~v; updateVal();}     //!< снимает значение из допустимых
  void addVal(int v) {val |= v; updateVal();}     //!< добавляет значение
  void swtVal(int v) {val ^= v; updateVal();}     //!< переключает значение
  // выделение
  bool isSelected()const {return inselect;}       //!< возвращает флаг выбранности
  void setSelected(bool selected=true);           //!< устанавливает флаг выбранности
protected:
  void focusInEvent(QFocusEvent *ev) {setSelected(true); QLabel::focusInEvent(ev);}
  void focusOutEvent(QFocusEvent *ev) {setSelected(false); QLabel::focusOutEvent(ev);}
  void keyPressEvent(QKeyEvent *ev);
private:
  int val;
  bool inselect;
  void updateVal(); //!< обновляет отображение
};

#endif // MWIND_H
