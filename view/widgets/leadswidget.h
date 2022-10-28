#ifndef LEADSWIDGET_H
#define LEADSWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QMenu>
#include <QString>

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
  void display_data();
//  void handle_context_menu(const QPoint &pos);

 private:
  Ui::LeadsWidget *ui;
  Controller *controller_;
  QUrlQuery params_;
  void SetParams();
//  void ShowItem(QJsonObject obj);
};

#endif  // LEADSWIDGET_H
