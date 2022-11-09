#ifndef LEADSWIDGET_H
#define LEADSWIDGET_H

#include <QDialog>
#include <QEvent>
#include <QHelpEvent>
#include <QLineEdit>
#include <QMenu>
#include <QString>

#include "controller/controller.h"
#include "tablewidget.h"

namespace Ui {
class LeadsWidget;
}  // namespace Ui

class ArrayTableView;

class LeadsWidget : public QDialog {
  Q_OBJECT

 public:
  explicit LeadsWidget(Controller *controller, QWidget *parent = nullptr);
  ~LeadsWidget();

 private slots:
  void on_chb_a_toggled(bool checked);
  void on_chb_b_toggled(bool checked);
  void on_btn_OK_clicked();

 private:
  Ui::LeadsWidget *ui;
  Controller *controller_;
  QSqlRelationalTableModel *model_;
  QUrlQuery params_;
  ArrayTableView *delegate_;
  void SetParams();
};

class ArrayTableView : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit ArrayTableView(QObject *parent = nullptr);
  ~ArrayTableView();

  ArrayTableView(QObject *parent = nullptr, QSqlRelationalTableModel *model = nullptr);
  bool editorEvent(QEvent *event, QAbstractItemModel *model,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) override;

 private:
  ArrayTableView *ui;
  QSqlRelationalTableModel *model_;
};

#endif  // LEADSWIDGET_H
