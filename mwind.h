#ifndef MWIND_H
#define MWIND_H

#include <QWidget>
#include <QVector>
#include <QLabel>

//#include<QDebug>


//! главное окно программы
/*! Окно отображает матрицу ячеек. Основное назначение являться контейнером для
    расположения ячеек и их начального создания. */
class MWind : public QWidget
{
  Q_OBJECT
public:
  enum {
    SubSquare = 3,                //!< размер малого квадрата
    SideSize=SubSquare*SubSquare, //!< размер стороны, максимум определяется размерностью int-1
    MatrixSize=SideSize*SideSize  //!< количество ячеек матрицы
  };
                    //! функция отображения окна. что более?
  static void showInst() {(new MWind)->show();}
  class Cell;       //!< ячейка матрицы для отображения на экране
  enum RelationGroup {
    RGHoriz = 1,    //!< ячейки горизонтальной группы
    RGVert  = 2,    //!< ячейки вертикальной группы
    RGSubSq = 4,    //!< ячейки малого квадрата
    RGAll   = 7     //!< все группы вместе
  };
                    //! класс флагов групп
  Q_DECLARE_FLAGS(RelationGroups,RelationGroup)
                    //! отображает окно выбора.
                    /*! @return 0 - нет выбора; -1 - сброс значения; или сам выбор. */
  static int popupEnterForm(const QString& title);
protected:
  //! на тот случай если окно будет не главным или их будет несколько
  void closeEvent(QCloseEvent *e) {QWidget::closeEvent(e);deleteLater();}
private:
  explicit MWind(QWidget *parent = 0);
  QVector<Cell*> cells;               //!< общий набор ячеек @todo возможно лишнее, убрать
};

// ячейка матрицы для отображения на экране
/*!
  Основной функционал поведения закладывается в этом классе. Он производит
  отображение ячейки, принимает управление.
 */
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
  int value() const {return val;}//!< возвращает хранилище значений
  void setAllValues() {val = (1<<SideSize)-1; updateVal();} //!< сбрасывает значения на все допустимые
  void swtVal(int v) { val ^= v; updateVal(true);}//!< переключает значение
  void setVal(int v) {val = v; updateVal(true);}  //!< устанавливает значение
  // выделение
  bool isSelected()const {return inselect;}       //!< возвращает флаг выбранности
  void setSelected(bool selected=true);           //!< устанавливает флаг выбранности
  // для валидатора..
                                                  //! возвращает связанные ячейки
  QList<Cell*> relatedCells(RelationGroups group=RGAll)const;
  int isDeterminans()const;         //!< если значение определено (единственный вариант) возвращает его, иначе 0
protected:
  void focusInEvent(QFocusEvent *ev) {setSelected(true); QLabel::focusInEvent(ev);}
  void focusOutEvent(QFocusEvent *ev) {setSelected(false); QLabel::focusOutEvent(ev);}
  void keyPressEvent(QKeyEvent *ev);
  void mousePressEvent(QMouseEvent *ev);
  void mouseDoubleClickEvent(QMouseEvent *ev);
private:
  int val;
  bool inselect;
  void updateVal(bool updateRelated=false); //!< обновляет отображение
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MWind::RelationGroups)

#endif // MWIND_H
