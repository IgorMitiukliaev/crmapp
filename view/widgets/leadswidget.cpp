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
  connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),
          SLOT(customMenuRequested(QPoint)));
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

  QDate addressDateFrom;
  QDate addressDateTo;
  QDate createdFrom;
  QDate createdTo;
  QDate dateTimeFrom;

  if (ui->chb_a->isChecked()) {
    addressDateFrom = ui->addressDateFrom->date();
    addressDateTo = ui->addressDateTo->date();
    params_.addQueryItem("addressDateFrom",
                         addressDateFrom.toString(Qt::ISODateWithMs));
    params_.addQueryItem("addressDateTo",
                         addressDateTo.toString(Qt::ISODateWithMs));
  }

  if (ui->chb_b->isChecked()) {
    createdFrom = ui->createdFrom->date();
    createdTo = ui->createdTo->date();
    params_.addQueryItem("createdFrom",
                         createdFrom.toString(Qt::ISODateWithMs));
    params_.addQueryItem("createdTo", createdTo.toString(Qt::ISODateWithMs));
  }

  if (ui->chb_hist->isChecked()) {
    if (ui->chb_a->isChecked() && ui->chb_b->isChecked()) {
      dateTimeFrom = std::min(addressDateFrom, createdFrom);
    } else if (ui->chb_a->isChecked()) {
      dateTimeFrom = addressDateFrom;
    } else if (ui->chb_b->isChecked()) {
      dateTimeFrom = createdFrom;
    }

    if (ui->chb_a->isChecked() || ui->chb_b->isChecked()) {
      params_.addQueryItem("dateTimeFrom",
                           dateTimeFrom.toString(Qt::ISODateWithMs));
    }
    if (params_.hasQueryItem("Id"))
      params_.addQueryItem("LeadId", params_.queryItemValue("Id"));
    params_.addQueryItem("take", "9999");
  }

  //  qDebug() << params_.toString();
}

void LeadsWidget::on_btn_get_history__modify_lead_status_clicked() {}

void LeadsWidget::on_btn_get_ed_unit_students_clicked() {}

void LeadsWidget::customMenuRequested(QPoint pos) {
  int column = ui->tableView->horizontalHeader()->logicalIndexAt(pos);
  qDebug() << column;
  QMenu *menu=new QMenu(this);
  menu->addSection(QString::number(column));
  menu->popup(ui->tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}
