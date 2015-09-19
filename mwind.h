#ifndef MWIND_H
#define MWIND_H

#include <QWidget>
#include <QVector>

//! главное окно программы
class MWind : public QWidget
{
  Q_OBJECT
public:
  static void showInst() {(new MWind)->show();} //!< функция отображения окна. что более?
protected:
  void closeEvent(QCloseEvent *e) {QWidget::closeEvent(e);deleteLater();} //!< надо же как-то чистить память
private:
  explicit MWind(QWidget *parent = 0);
  class Cell;                         //!< ячейка матрицы для отображения на экране
  QVector<Cell*> cells;               //!< общий набор ячеек
};



#endif // MWIND_H
