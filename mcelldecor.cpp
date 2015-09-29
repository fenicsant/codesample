#include "mcelldecor.h"
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QStringList>

#include "msudoku.h"

class MCellDecor::Editor :public QWidget
{
public:
  Editor(MSudoku * model_,const QModelIndex & index_,QWidget *parent);
  MSudoku *model;
  const QModelIndex & index;
  MCellData data;
  QGridLayout *lay;
  QVector<Label*> btns;
  void btnclicked(int num);
  void btndblclicked(int num);
protected:
  void keyPressEvent(QKeyEvent *ev);
};

class MCellDecor::Label :public QLabel
{
public:
  Label(Editor *parent,int number) :
        QLabel(QString::number(number),parent),editor(parent),num(number)
        { setAlignment(Qt::AlignCenter); }
  Editor * editor;
  int num;
  void setActive(bool active)
        {if (!active)setStyleSheet("background: white"); else setStyleSheet("background: silver");}
protected:
  void mousePressEvent(QMouseEvent *ev)
        {editor->btnclicked(num); QLabel::mousePressEvent(ev);}
  void mouseDoubleClickEvent(QMouseEvent *ev)
  {editor->btndblclicked(num); QLabel::mouseDoubleClickEvent(ev);}
};

MCellDecor::Editor::Editor(MSudoku * model_, const QModelIndex &index_, QWidget *parent) :
  QWidget(0,Qt::Popup),model(model_),index(index_),lay(new QGridLayout),btns(model_->sideSize())
{
  lay->setMargin(1);
  lay->setSpacing(1);
  data = model->cellValue(index);
  int side = sqrt(model->sideSize());
  for(int i=0; i<data.max; ++i) {
    Label *btn = btns[i] = new Label(this,i+1);
    lay->addWidget(btn,i/side,i%side);
    btn->setActive(data.testValue(i+1));
  }
  setStyleSheet("background: black");
  setLayout(lay);
}

void MCellDecor::Editor::btnclicked(int num)
{
  data.switchValue(num);
  btns[num-1]->setActive(data.testValue(num));
}

void MCellDecor::Editor::btndblclicked(int num)
{
  data.setValue(num);
  close();
}

void MCellDecor::Editor::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()>=Qt::Key_1 && ev->key()<=Qt::Key_9) {
    Qt::KeyboardModifiers mod = ev->modifiers() & ~Qt::KeypadModifier;
    if (mod == Qt::ControlModifier ) {
      btndblclicked(ev->key()-Qt::Key_1+1);
      return;
    }
    if (mod==Qt::NoModifier) {
      btnclicked(ev->key()-Qt::Key_1+1);
      return;
    }
  }
  QWidget::keyPressEvent(ev);
}


QFont& MCellDecor::roughFont()
{
  static QFont * res = new QFont("Arial",6);
  return *res;
}

QFont& MCellDecor::fairFont()
{
  static QFont * res = new QFont("Arial",12,QFont::Bold);
  return *res;
}

int MCellDecor::sqrt(int v)
{
  if (v<0) return 0;
  static QMap<int,int> cache;
  if (cache.contains(v)) return cache[v];
  int i=2;
  for(; i*i<v; ++i);
  cache[v]=i;
  return i;
}

int MCellDecor::sizeBySide(int valCount)
{
  static QMap<int,int> cache;
  if (cache.contains(valCount)) return cache.value(valCount);
  QFontMetrics fm(roughFont());
  int res=0;
  int squareSize = sqrt(valCount);

  for(int i=1; i<=valCount; ++i) {
    QRect r = fm.boundingRect(QString::number(i));
    if (r.width()>res) res = r.width();
    if (r.height()>res) res = r.height();
  }
  res += 4;
  res *= squareSize;

  fm = QFontMetrics(fairFont());
  for(int i=1; i<=valCount; ++i) {
    QRect r = fm.boundingRect(QString::number(i));
    if (r.width()>res) res = r.width();
    if (r.height()>res) res = r.height();
  }
  cache[valCount]=res;
  return res;
}

void MCellDecor::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  painter->setPen( (option.state & QStyle::State_Selected)
                   ?option.palette.highlightedText().color()
                   :option.palette.text().color()
                    );
  const MCellData & cell = model->cellValue(index);
  if (model->subSquareSize()>0) {
    int sss = model->subSquareSize();
    int x = index.column();
    int y = index.row();
    if (((x/sss%2)^(y/sss%2))!=0
        || option.state & QStyle::State_Selected)
      painter->fillRect(option.rect,
                        (option.state & QStyle::State_Selected)
                        ?option.palette.highlight()
                        :QBrush(Qt::lightGray)
                        );
    if ( x % sss == 0 && x != 0) {
      QPen p = painter->pen();
      p.setWidth(3);
      painter->setPen(p);
      painter->drawLine(option.rect.topLeft(),option.rect.bottomLeft());
    }
    if ( y % sss == 0 && y != 0) {
      QPen p = painter->pen();
      p.setWidth(3);
      painter->setPen(p);
      painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    }
  }
  if (cell.isOnlyValue()) {
    painter->setFont(fairFont());
    painter->drawText(option.rect,Qt::AlignCenter,QString::number(cell.onlyValue()));
    return;
  } else painter->setFont(roughFont());

  int sideDigCount = (model->subSquareSize()==0)?sqrt(model->sideSize()):model->subSquareSize();
  int left = option.rect.left()+2;
  int top = option.rect.top()+2;
  int hstep = (option.rect.width()-4)/sideDigCount;
  int vstep = (option.rect.height()-4)/sideDigCount;
  int max = model->sideSize();
  for(int i=0; i<max; ++i)
    if (cell.testValue(i+1)) {
      int x = i%sideDigCount;
      int y = i/sideDigCount;
      painter->drawText(QRect(x*hstep+left,top+y*vstep,hstep,vstep),Qt::AlignCenter,QString::number(i+1));
    }
}

QWidget *MCellDecor::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Editor *res = new Editor(model,index,parent);
  res->setGeometry(QRect(parent->mapToGlobal(option.rect.topLeft()),option.rect.size()));
  return res;
  return 0;
}

void MCellDecor::setModelData(QWidget *editor, QAbstractItemModel *model_, const QModelIndex &index) const
{
  Editor * e = dynamic_cast<MCellDecor::Editor*>(editor);
  if (!e || model!=model_) {
    QStyledItemDelegate::setModelData(editor,model,index);
    return;
  }
  model->setData(index,e->data,Qt::EditRole);
}

void MCellDecor::editFinished(Editor *editor)
{
  emit commitData(editor);
}
