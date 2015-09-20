/*! @author E. Nenilin
    @file mainproc.cpp файл явно лишний в этом проекте, надо было перенести
    функцию к окну. Но что сделано, то сделано - он остается.
 */

#include <QApplication>
#include "mwind.h"

//! точка входа в программу
int main(int ac,char **av)
{
  QApplication app(ac,av);
  MWind::showInst();
  return app.exec();
}
