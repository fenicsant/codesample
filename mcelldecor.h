#ifndef MCELLDECOR_H
#define MCELLDECOR_H

#include <QStyledItemDelegate>
#include <QFont>

class MSudoku;
//! производит отображение ячейки на экране
class MCellDecor : public QStyledItemDelegate
{
public:
          //!
  static QFont &roughFont();
  static QFont &fairFont();
  static int sqrt(int);
  static int sizeBySide(int valCount);
  explicit MCellDecor(MSudoku & model_,QObject *parent = 0) :
      QStyledItemDelegate(parent),model(&model_){}
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;
  void updateEditorGeometry(QWidget */*editor*/, const QStyleOptionViewItem &/*option*/,
                            const QModelIndex &/*index*/) const{}
  QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
private:
  MSudoku * model;
  class Label;
  class Editor;
public:
  void editFinished(Editor*editor);
};

#endif // MCELLDECOR_H
