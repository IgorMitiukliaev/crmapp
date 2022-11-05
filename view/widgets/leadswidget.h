#ifndef LEADSWIDGET_H
#define LEADSWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QMenu>
#include <QString>
#include <QEvent>

#include "controller/controller.h"

namespace Ui {
class LeadsWidget;
}

class LeadsWidget : public QDialog {
  Q_OBJECT

 public:
  explicit LeadsWidget(Controller *controller, QWidget *parent = nullptr);
  ~LeadsWidget();

 private slots:
  void on_chb_a_toggled(bool checked);
  void on_chb_b_toggled(bool checked);
  void on_btn_OK_clicked();
  //  void display_data();
  //  void display_sql_data();
  //  void handle_context_menu(const QPoint &pos);
  void on_btn_get_history__modify_lead_status_clicked();
  void on_btn_get_ed_unit_students_clicked();
  void customMenuRequested(QPoint);

private:
  Ui::LeadsWidget *ui;
  Controller *controller_;
  QSqlTableModel *model_;
  QUrlQuery params_;
  void SetParams();
  //  void ShowItem(QJsonObject obj);
};

#endif  // LEADSWIDGET_H
