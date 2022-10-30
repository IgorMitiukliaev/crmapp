#include "leadswidget.h"

#include "ui_leadswidget.h"

LeadsWidget::LeadsWidget(Controller *controller, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LeadsWidget),
      controller_(controller),
      model_(controller_->ShareModel()),
      params_() {
  QDate date = QDate::currentDate();
  ui->setupUi(this);
  ui->addressDateFrom->setDate(date);
  ui->addressDateTo->setDate(date);
  ui->createdTo->setDate(date);
  ui->createdFrom->setDate(date);
  ui->tableView->setModel(model_);
  //  ui->tableView->resizeColumnsToContents();
  //  connect(controller_, &Controller::dataReady, this,
  //          &LeadsWidget::display_data);
  //  connect(model_, &QSqlRelationalTableModel::dataChanged, this, [this]() {
  //    qDebug() << "\n\nROWS from widget = " << model_->rowCount();
  //    model_->select();
  //    ui->tableView->update();
  //  });
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
  if (ui->chb_hist->isChecked())
    controller_->GetGetHistoryModifyLeadStatus(params_);
  else
    controller_->GetDataFromApi("GetLeads", params_);
}

// void LeadsWidget::display_data() {
//   QJsonArray leadsArray = controller_->GetJsonData();
//   // tableWidget has been set here
//   QTableWidget *tbl = ui->tableWidget;
//   tbl->clear();
//   QStringList labels{
//       "Id",        "LastName",   "FirstName", "Discipline",
//       "Status",    "Created",    "Agents",    "OfficesAndCompanies",
//       "Assignees", "ExtraFields"};

//  tbl->setColumnCount(labels.size());
//  tbl->setRowCount(1);
//  tbl->setHorizontalHeaderLabels(labels);

//  int r = 0;
//  for (const QJsonValue &val : leadsArray) {
//    QJsonObject obj = val.toObject();
//    int c = 0;
//    for (const QString &key : labels) {
//      QString value;
//      //      qDebug() << "obj [" << key << "] = " << obj[key];
//      if (obj[key].type() == QJsonValue::Double)
//        value = QString::number(obj[key].toDouble());
//      else
//        value = obj[key].toString();
//      auto *it = new QTableWidgetItem(value);
//      tbl->setItem(r, c, it);
//      c++;
//    }
//    tbl->insertRow(++r);
//  }
//}

void LeadsWidget::SetParams() {
  params_.clear();
  QString value = ui->id->text();
  if (value != "") params_.addQueryItem("Id", value);

  value = ui->studentClientId->text();
  if (value != "") params_.addQueryItem("studentClientId", value);

  value = ui->term->text();
  if (value != "") {
    params_.addQueryItem("term", value);
    value = ui->byAgents->isChecked() ? "true" : "false";
    params_.addQueryItem("byAgents", value);
  }

  int attached = ui->attached->currentIndex();
  switch (attached) {
    case 1:
      params_.addQueryItem("attached", "true");
      break;
    case 2:
      params_.addQueryItem("attached", "false");
      break;
    case 0:
    default:
      params_.addQueryItem("attached", "null");
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
  //  qDebug() << params_.toString();
}
