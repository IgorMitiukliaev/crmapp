#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QDialog>
#include <QSqlQueryModel>


namespace Ui {
class TableWidget;
}

class TableWidget : public QDialog {
  Q_OBJECT

 public:
  explicit TableWidget(QWidget *parent = nullptr, QAbstractItemModel *model = nullptr);
  ~TableWidget();

private slots:
  void on_btn_OK_accepted();

private:
  Ui::TableWidget *ui;
  QSqlQueryModel *model_;
};

#endif  // TABLEWIDGET_H
