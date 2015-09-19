/*! @author E. Nenilin
 */

#include <QApplication>
#include "mwind.h"

#include <QDebug>
int main(int ac,char **av)
{
  QApplication app(ac,av);
  MWind::showInst();
  return app.exec();
}
