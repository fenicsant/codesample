#include "mwind.h"

#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QProgressBar>

#include "cellvalidator.h"


MWind::MWind(QWidget *parent) :
  QWidget(parent), cells(MatrixSize,0)
{
  QProgressBar progres;
  progres.setMaximum(MatrixSize);
  progres.setValue(0);
  progres.show();

  QGridLayout *lay = new QGridLayout();
  lay->setMargin(1);
  lay->setSpacing(1);
  for(int i=0; i<MatrixSize; ++i) {
    int x = i%SideSize;
    int y = i/SideSize;
    lay->addWidget(cells[i]=new Cell(this,cells,x,y),y+y/SubSquare,x+x/SubSquare);
    progres.setValue(i);
  }
  setStyleSheet("background: black");
  setLayout(lay);

  for(int i=0; i<SubSquare-1; ++i) {
    lay->setRowMinimumHeight(i*(SubSquare+1)+SubSquare,1);
    lay->setColumnMinimumWidth(i*(SubSquare+1)+SubSquare,1);
  }
  progres.close();
}

/*  ****************** @class MWind::Cell ****************** */

MWind::Cell::Cell(QWidget * parent, QVector<Cell*> &cells_, int x_, int y_)
  :QLabel(parent),cells(cells_),x(x_),y(y_)
{
  setFocusPolicy(Qt::TabFocus);
  setAllValues();
  setSelected(false);
  relatedCells();
  static int fsize = fontMetrics().height()*SubSquare+9;
  setMinimumSize(fsize,fsize);
}

void MWind::Cell::setSelected(bool selected)
{
  setStyleSheet((inselect=selected)?QString("background: cyan"):QString("background: white"));
}

QList<MWind::Cell *> MWind::Cell::relatedCells() const
{
  QList<Cell*> res;
  int ssx = (x/SubSquare)*SubSquare;
  int ssy = (y/SubSquare)*SubSquare;
  for(int i=SideSize-1; i>=0; --i) {
    if (i!=x) res.append(&byCoord(i,y));
    if (i!=y) res.append(&byCoord(x,i));
    int sx = i%SubSquare+ssx;
    int sy = i/SubSquare+ssy;
    if (sx!=x && sy!=y) res.append(&byCoord(sx,sy));
  }
  res.removeAll(0);
  return res;
}

int MWind::Cell::isDeterminans() const
{
  int i=1;
  int v2=val;
  while (!(v2&1)) {v2>>=1; ++i;}
  return (!(v2 >> 1))?i:0;
}

void MWind::Cell::keyPressEvent(QKeyEvent *ev)
{
  QLabel::keyPressEvent(ev);
  int key = ev->key();
  switch (key) {
  case Qt::Key_Left: if (x>0) byCoord(x-1,y).setFocus(); break;
  case Qt::Key_Up:   if (y>0) byCoord(x,y-1).setFocus(); break;
  case Qt::Key_Right:if (x<SideSize-1) byCoord(x+1,y).setFocus(); break;
  case Qt::Key_Down: if (y<SideSize-1) byCoord(x,y+1).setFocus(); break;
  default: {
    if (key>=Qt::Key_1 && key<=Qt::Key_9) {
      if (ev->modifiers()&Qt::ControlModifier)
        setVal(1<<(key-Qt::Key_1));
      else swtVal(1<<(key-Qt::Key_1));
    }
  }
  }
}

void MWind::Cell::updateVal(bool updateRelated)
{
  if (val == 0) {
    setText(QString());
    return;
  }
  int determinans = isDeterminans();
  if (determinans!=0){
    QString color = "black";
    if (ICellValidator::cellValdator()) {
      switch (ICellValidator::cellValdator()->check(*this,determinans)) {
      case ICellValidator::ValueBad: color = "red"; break;
      case ICellValidator::ValueRecomend: color = "green"; break;
      default:;
      }
    }
    setText(QString("<h2 align=center><font color=%1>%2</font></h2>").arg(color).arg(determinans));
  } else {
    QString ntext="<body><table width=100% hegth=100%>";
    int mask=1;
    for(int i=0; i<SideSize; ++i, mask<<=1) {
      int x=i%SubSquare;
      if (x==0) ntext.append("<tr>");
      QString color = "black";
      if (ICellValidator::cellValdator()) {
        switch (ICellValidator::cellValdator()->check(*this,i+1)) {
        case ICellValidator::ValueBad: color = "#FF0F0F"; break;
        case ICellValidator::ValueRecomend: color = "green"; break;
        default:;
        }
      }
      ntext.append(QString("<td align=center><font color=%1>%2</font>").arg(color).arg(((mask&val)?QString::number(i+1):QString(" "))));
    }
    setText(ntext+"</table></body>");
  }
  if (updateRelated) {
    foreach(Cell *r,relatedCells()) if (r)r->updateVal();
  }
}
