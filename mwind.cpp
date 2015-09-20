#include "mwind.h"

#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QProgressBar>
#include <QApplication>

#include "cellvalidator.h"

namespace MWindCppHideNS
{
class LabelButton;
class PopupWind : public QWidget
{
public:
  QLabel * titleLabel;
  int result;
  explicit PopupWind();
  void btnDown(LabelButton*b);
};
class LabelButton : public QLabel
{
public:
  PopupWind *own;
  int val;
  explicit LabelButton(PopupWind *o,int text)
    :QLabel(QString::number(text),o),own(o),val(text)
      {setStyleSheet("background: white");setAlignment(Qt::AlignCenter);}
protected:
  void mousePressEvent(QMouseEvent *) {own->btnDown(this);}
};
PopupWind::PopupWind() :
  QWidget(0,Qt::Popup)
{
  QGridLayout *lay = new QGridLayout;
  titleLabel = new QLabel(this);
  titleLabel->setStyleSheet("background: silver");
  lay->addWidget(titleLabel,0,0,1,MWind::SubSquare);
  for(int i=0; i<MWind::SideSize; ++i) {
    lay->addWidget(new LabelButton(this,i+1),i/MWind::SubSquare+1,i%MWind::SubSquare);
  }
  LabelButton *lb = 0;
  lay->addWidget(lb = new LabelButton(this,-1),MWind::SubSquare+1,0,1,MWind::SubSquare);
  lb->setText(QString::fromUtf8("Сбросить"));
  lay->setMargin(1);
  lay->setSpacing(1);
  setStyleSheet("background: black");
  setLayout(lay);
}
void PopupWind::btnDown(LabelButton *b)
{
  result=b->val;
  close();
}
}
using namespace MWindCppHideNS;

MWind::MWind(QWidget *parent) :
  QWidget(parent), cells(MatrixSize,0)
{
  setWindowTitle(QString::fromUtf8("Черновик судоку. Перемещение-стрелки. Ввод-цифры и Ctrl+цифры."));

  QProgressBar progres;
  progres.setMaximum(MatrixSize);
  progres.setValue(0);
  progres.setWindowTitle(QString::fromUtf8("Подождите, идет размещение"));
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

int MWind::popupEnterForm(const QString &title)
{
  static PopupWind * wind = new PopupWind();
  wind->titleLabel->setText(title);
  wind->result = 0;
  wind->move(QCursor::pos());
  wind->show();
  while (wind->isVisible()) qApp->processEvents();
  return wind->result;
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


QList<MWind::Cell *> MWind::Cell::relatedCells(RelationGroups group) const
{
  QList<Cell*> res;
  int ssx = (x/SubSquare)*SubSquare;
  int ssy = (y/SubSquare)*SubSquare;
  for(int i=0; i<SideSize; ++i) {
    if (i!=x && group.testFlag(RGHoriz)) res.append(&byCoord(i,y));
    if (i!=y && group.testFlag(RGVert)) res.append(&byCoord(x,i));
    if (group.testFlag(RGSubSq)) {
      int sx = i%SubSquare+ssx;
      int sy = i/SubSquare+ssy;
      if (sx!=x && sy!=y) res.append(&byCoord(sx,sy));
    }
  }
  res.removeAll(0);
  return res;
}

int MWind::Cell::isDeterminans() const
{
  int i=1;
  if (!val) return 0;
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

void MWind::Cell::mousePressEvent(QMouseEvent *ev)
{
  QLabel::mousePressEvent(ev);
  if (ev->button()==Qt::LeftButton) {
    setFocus();
    return;
  }
  if (ev->button()==Qt::RightButton) {
    int res = popupEnterForm(QString::fromUtf8("Переключить значение ячейки"));
    if (res==0) return;
    if (res == -1) {
      setAllValues();
      updateVal(true);
      return;
    }
    if (res<1 || res>SideSize) return;
    swtVal(1<<(res-1));
  }
}

void MWind::Cell::mouseDoubleClickEvent(QMouseEvent *ev)
{
  QLabel::mouseDoubleClickEvent(ev);
  int res = popupEnterForm(QString::fromUtf8("Установить значение ячейки"));
  if (res == 0) return;
  if (res == -1){
    setAllValues();
    updateVal(true);
    return;
  }
  if (res<1 || res>SideSize) return;
  setVal(1<<(res-1));
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
      //case ICellValidator::ValueRecomend: color = "green"; break;
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
        case ICellValidator::ValueBad: color = "silver size=-2"; break;
        case ICellValidator::ValueRecomend: color = "green size=+2"; break;
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

