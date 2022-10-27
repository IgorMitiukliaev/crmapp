#include "leadswidget.h"

#include "ui_leadswidget.h"

LeadsWidget::LeadsWidget(Controller *controller, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LeadsWidget),
      controller_(controller),
      params_() {
  QDate date = QDate::currentDate();
  ui->setupUi(this);
  ui->addressDateFrom->setDate(date);
  ui->addressDateTo->setDate(date);
  ui->createdTo->setDate(date);
  ui->createdFrom->setDate(date);
}

LeadsWidget::~LeadsWidget() { delete ui; }

void LeadsWidget::on_chb_a_toggled(bool checked) {
  ui->addressDateFrom->setEnabled(checked);
  ui->addressDateTo->setEnabled(checked);
}
void LeadsWidget::on_chb_b_toggled(bool checked) {
  ui->createdTo->setEnabled(checked);
  ui->createdFrom->setEnabled(checked);
}

void LeadsWidget::on_btn_OK_clicked() {
  SetParams();
  controller_->GetLeads(params_);
}

void LeadsWidget::SetParams() {
  params_.clear();
  QString value = ui->id->text();
  if (value != "") params_.addQueryItem("id", value);

  value = ui->studentClientId->text();
  if (value != "") params_.addQueryItem("studentClientId", value);

  value = ui->term->text();
  if (value != "") params_.addQueryItem("term", value);

  value = ui->byAgents->isChecked() ? "true" : "false";
  params_.addQueryItem("byAgents", value);

  int attached = ui->attached->currentIndex();
  switch (attached) {
    case 1:
      params_.addQueryItem("attached", "null");
      break;
    case 2:
      params_.addQueryItem("attached", "true");
      break;
    case 0:
    default:
      params_.addQueryItem("attached", "false");
  };

  if (ui->chb_a->isChecked()) {
    QString addressDateFrom =
        ui->addressDateFrom->date().toString(Qt::ISODateWithMs);
    QString addressDateTo =
        ui->addressDateTo->date().toString(Qt::ISODateWithMs);
    params_.addQueryItem("addressDateFrom", addressDateFrom);
    params_.addQueryItem("addressDateTo", addressDateTo);
  }

  if (ui->chb_b->isChecked()) {
    QString createdFrom = ui->createdFrom->date().toString(Qt::ISODateWithMs);
    QString createdTo = ui->createdTo->date().toString(Qt::ISODateWithMs);
    params_.addQueryItem("createdFrom", createdFrom);
    params_.addQueryItem("createdTo", createdTo);
  }
  qDebug() << params_.toString();
}
