/*! @author E. Nenilin
    @file mainproc.cpp файл явно лишний в этом проекте, надо было перенести
    функцию к окну. Но что сделано, то сделано - он остается.
 */

#include <QApplication>
//#include "mwind.h"

#include "cell.h"
#include "celldecoration.h"
#include <QTableView>

//! точка входа в программу
int main(int ac,char **av)
{
  QApplication app(ac,av);
  //MWind::showInst();
  CellMatrix *model = CellMatrix::createBySubSquareSize(3);
  QTableView tv;
  tv.setModel(model);
  tv.setItemDelegate(new CellDecoration());
  tv.show();
  return app.exec();
}
