#ifndef MSUDOKUDECARATION_H
#define MSUDOKUDECARATION_H

#include <QStyledItemDelegate>
#include <QFont>

class MSudokuDecoration : public QStyledItemDelegate
{
  Q_OBJECT
public:
  static QFont &roughFont();
  static QFont &fairFont();
  static int sqrt(int);
  explicit MSudokuDecoration(QObject *parent = 0) : QStyledItemDelegate(parent){}
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:
private:
  class Label;
  class Editor;
};

#endif // MSUDOKUDECARATION_H
