#include "mwind.h"
#include "cell_p.h"

#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>

MWind::MWind(QWidget *parent) :
  QWidget(parent), cells(9)
{
  QGridLayout *lay = new QGridLayout();
  lay->setMargin(1);
  lay->setSpacing(1);
  for(int i=0; i<9; ++i) {
    int x = i%3;
    int y = i/3;
    lay->addWidget(cells[i]=new Cell(this,cells,x,y),y,x);
  }
  setStyleSheet("background: black");
  setLayout(lay);
}

/*  ****************** @class MWind::Cell ****************** */

MWind::Cell::Cell(QWidget * parent, QVector<Cell*> &cells_, int x_, int y_)
  :QLabel(parent),cells(cells_),x(x_),y(y_)
{
  setFocusPolicy(Qt::TabFocus);
  setAllValues();
  setSelected(false);
  static int fsize = fontMetrics().height()*3+9;
  setMinimumSize(fsize,fsize);
}

void MWind::Cell::setSelected(bool selected)
{
  setStyleSheet((inselect=selected)?QString("background: cyan"):QString("background: white"));
}

void MWind::Cell::keyPressEvent(QKeyEvent *ev)
{
  QLabel::keyPressEvent(ev);
  int key = ev->key();
  switch (key) {
  case Qt::Key_Left: if (x>0) byCoord(x-1,y).setFocus(); break;
  case Qt::Key_Up:   if (y>0) byCoord(x,y-1).setFocus(); break;
  case Qt::Key_Right:if (x<2) byCoord(x+1,y).setFocus(); break;
  case Qt::Key_Down: if (y<2) byCoord(x,y+1).setFocus(); break;
  default: {
    if (key>=Qt::Key_1 && key<=Qt::Key_9) swtVal(1<<(key-Qt::Key_1));
  }
  }
}

void MWind::Cell::updateVal()
{
  if (val == 0) {
    setText(QString());
    return;
  }
  int i=1;
  int v2=val;
  while (!(v2&1)) {v2>>=1; ++i;}
  if (!(v2 >> 1)) {
    setText(QString("<h2 align=center>%1</h2>").arg(i));
  } else {
    QString ntext="<body><table width=100% hegth=100%>";
    int mask=1;
    for(int i=0; i<9; ++i, mask<<=1) {
      int x=i%3;
      if (x==0) ntext.append("<tr>");
      ntext.append("<td align=center>"+((mask&val)?QString::number(i+1):QString(" ")));
    }
    setText(ntext+"</table></body>");
  }
}
