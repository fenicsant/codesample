#include "celldecoration.h"
#include "cell.h"
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QKeyEvent>

class CellDecoration::Editor : public QWidget
{
public:
  const CellDecoration * own;
  CellData *cell;
  Editor(const CellDecoration * own_, QWidget *parent, int max, int side, CellData *cell_);
  QVector<Label *> btns;
  void btnPressed(int num);
  void btnDblPressed(int num) {
    cell->setValue(num);
    cell->switchValue(num);
    close();
  }
protected:
  void keyPressEvent(QKeyEvent *ev) {
    if (ev->key()>=Qt::Key_1 && ev->key()<=Qt::Key_9) {
      Qt::KeyboardModifiers mod = ev->modifiers() & ~Qt::KeypadModifier;
      if (mod == Qt::ControlModifier ) {
        btnDblPressed(ev->key()-Qt::Key_1+1);
        return;
      }
      if (mod==Qt::NoModifier) {
        btnPressed(ev->key()-Qt::Key_1+1);
        return;
      }
    }
    QWidget::keyPressEvent(ev);
  }
};

class CellDecoration::Label : public QLabel
{
public:
  Editor *editor;
  int value;
  void setActive(bool active) {if (!active)setStyleSheet("background: white"); else setStyleSheet("background: silver");}
  Label(Editor *e,int value_):QLabel(QString::number(value_),e),editor(e),value(value_)
  { setAlignment(Qt::AlignCenter); }
protected:
  void mousePressEvent(QMouseEvent *ev) {editor->btnPressed(value); QLabel::mousePressEvent(ev);}
  void mouseDoubleClickEvent(QMouseEvent *ev) {editor->btnDblPressed(value); QLabel::mouseDoubleClickEvent(ev);}
};

CellDecoration::Editor::Editor(const CellDecoration *own_, QWidget *parent, int max, int side, CellData *cell_) :
  QWidget(0,Qt::Popup),own(own_),cell(cell_)
{
  setStyleSheet("background: black");
  QGridLayout *lay = new QGridLayout;
  lay->setSpacing(1);
  lay->setMargin(1);
  btns.resize(max);
  for(int i=0; i<max; ++i) {
    lay->addWidget(btns[i]=new Label(this,i+1),i/side,i%side);
    btns[i]->setActive(cell->testValue(i+1));
  }
  setLayout(lay);
}

void CellDecoration::Editor::btnPressed(int num)
{
  cell->switchValue(num);
  btns[num-1]->setActive(cell->testValue(num));
  qDebug()<<*cell;
}


void CellDecoration::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  CellData * cell = static_cast<CellData*>(index.internalPointer());
  if (!cell) {QStyledItemDelegate::paint(painter,option,index); return;}


  painter->setPen( (option.state & QStyle::State_Selected)
                   ?option.palette.highlightedText().color()
                   :option.palette.text().color()
                    );
  if (cell->matrix()->subSquareSize()>0) {
    int sss = cell->matrix()->subSquareSize();
    if (((cell->x/sss%2)^(cell->y/sss%2))!=0
        || option.state & QStyle::State_Selected)
      painter->fillRect(option.rect,
                        (option.state & QStyle::State_Selected)
                        ?option.palette.highlight()
                        :QBrush(Qt::lightGray)
                        );
    if ( cell->x % sss == 0 && cell->x != 0) {
      QPen p = painter->pen();
      p.setWidth(3);
      painter->setPen(p);
      painter->drawLine(option.rect.topLeft(),option.rect.bottomLeft());
    }
    if ( cell->y % sss == 0 && cell->y != 0) {
      QPen p = painter->pen();
      p.setWidth(3);
      painter->setPen(p);
      painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    }
  }
  if (cell->isOnlyValue()) {
    painter->setFont(fairFont());
    painter->drawText(option.rect,Qt::AlignCenter,QString::number(cell->onlyValue()));
    return;
  } else painter->setFont(roughFont());

  int sideDigCount = (cell->matrix()->subSquareSize()==0)?sqrt(cell->matrix()->sideSize()):cell->matrix()->subSquareSize();
  int left = option.rect.left()+2;
  int top = option.rect.top()+2;
  int hstep = (option.rect.width()-4)/sideDigCount;
  int vstep = (option.rect.height()-4)/sideDigCount;
  int max = cell->matrix()->sideSize();
  for(int i=0; i<max; ++i)
    if (cell->testValue(i+1)) {
      int x = i%sideDigCount;
      int y = i/sideDigCount;
      painter->drawText(QRect(x*hstep+left,top+y*vstep,hstep,vstep),Qt::AlignCenter,QString::number(i+1));
    }
}

QWidget *CellDecoration::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  CellData * cell = static_cast<CellData*>(index.internalPointer());
  if (!cell) {return QStyledItemDelegate::createEditor(parent,option,index); }

  Editor *res = new Editor(this,parent,cell->matrix()->sideSize(),(cell->matrix()->subSquareSize()==0)?sqrt(cell->matrix()->sideSize()):cell->matrix()->subSquareSize(),cell);
  res->setGeometry(QRect(parent->mapToGlobal(option.rect.topLeft()),option.rect.size()));
  return res;
}

void CellDecoration::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

QFont &CellDecoration::roughFont()
{
  static QFont * res = new QFont("Arial",6);
  return *res;
}

QFont &CellDecoration::fairFont()
{
  static QFont * res = new QFont("Arial",12,QFont::Bold);
  return *res;
}

int CellDecoration::sqrt(int v)
{
  if (v<0) return 0;
  static QMap<int,int> cache;
  if (cache.contains(v)) return cache[v];
  int i=2;
  for(; i*i<v; ++i);
  cache[v]=i;
  return i;
}




// Если в сети нет пингЫ, виноваты админЫ
