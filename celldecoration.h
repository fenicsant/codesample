#ifndef CELLDECORATION_H
#define CELLDECORATION_H

#include <QStyledItemDelegate>

class CellDecoration : public QStyledItemDelegate
{
  Q_OBJECT
public:
  static QFont &roughFont();
  static QFont &fairFont();
  static int sqrt(int);
  explicit CellDecoration(QObject *parent = 0) : QStyledItemDelegate(parent){}
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
  
public slots:
private:
  class Label;
  class Editor;
};


#endif // CELLDECORATION_H
