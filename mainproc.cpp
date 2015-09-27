/*! @author E. Nenilin
    @file mainproc.cpp файл явно лишний в этом проекте, надо было перенести
    функцию к окну. Но что сделано, то сделано - он остается.
 */

#include <QApplication>

#include <QTableView>
#include "msudoku.h"
#include "msudokudecaration.h"

//! точка входа в программу
int main(int ac,char **av)
{
  QApplication app(ac,av);
  //MWind::showInst();
  MSudoku *model = MSudoku::createBySubSquareSize(3);
  QTableView tv;
  tv.setModel(model);
  tv.setItemDelegate(new MSudokuDecoration());
  int size = (model->cellHeight>model->cellWidth)?model->cellHeight:model->cellWidth;
  for(int i=model->sideSize()-1; i>=0; --i) {
    tv.setRowHeight(i,size);
    tv.setColumnWidth(i,size);
  }
  tv.show();
  return app.exec();
}
