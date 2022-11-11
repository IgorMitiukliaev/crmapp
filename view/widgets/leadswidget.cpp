#include "leadswidget.h"

#include "ui_leadswidget.h"

using ::ArrayTableView;
using ::LeadsWidget;

LeadsWidget::LeadsWidget(Controller *controller, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LeadsWidget),
      controller_(controller),
      model_(controller_->ShareModel()),
      params_() {
  ui->setupUi(this);
  ui->tableView->setModel(model_);
  delegate_ = new ArrayTableView(this, model_);
  ui->tableView->setItemDelegate(delegate_);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->setSortingEnabled(true);
  ui->tableView->horizontalHeader()->setSectionsMovable(true);

  QDate date = QDate::currentDate();
  ui->addressDateFrom->setDate(date);
  ui->addressDateTo->setDate(date);
  ui->createdTo->setDate(date);
  ui->createdFrom->setDate(date);
}

void LeadsWidget::keyPressEvent(QKeyEvent *event) {
  if (event->matches(QKeySequence::Copy)) {
    QString text;
    QItemSelectionRange range =
        ui->tableView->selectionModel()->selection().first();
    for (auto i = range.top(); i <= range.bottom(); ++i) {
      QStringList rowContents;
      for (auto j = range.left(); j <= range.right(); ++j) {
        QString buffer = model_->index(i, j).data().toString();
        rowContents += buffer.length() > 0 ? buffer : " ";
      }
      text += rowContents.join("\t");
      text += "\n";
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text, QClipboard::Clipboard);
  }
}

void LeadsWidget::on_pushButton_clicked() { GetLeadsStats(); }

void LeadsWidget::GetLeadsStats() {
  QMap<QString, QString> stats = controller_->GetLeadsStats();
  QString text;
  text = "Всего лидов: " + stats["Leads"] + "\n";
  text += "Лидов с пробными уроками: " + stats["EdUnitLeads"] + "\n";
  ui->lbl_info->setText(text);
};

LeadsWidget::~LeadsWidget() {
  delete ui;
  delete delegate_;
}

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
  controller_->ClearDb();
  if (ui->chb_hist->isChecked())
    controller_->GetFullLeadsData(params_);
  else
    controller_->GetDataFromApi("GetLeads", params_);
}

void LeadsWidget::SetParams() {
  QDate addressDateFrom;
  QDate addressDateTo;
  QDate createdFrom;
  QDate createdTo;
  //  QDate dateTimeFrom;

  params_.clear();
  QString value = ui->id->text();
  params_.addQueryItem("take", "9999");
  if (value != "") params_.addQueryItem("Id", value);

  value = ui->term->text();
  if (value != "") {
    params_.addQueryItem("term", value);
    params_.addQueryItem("byAgents", "true");
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
}

//
// Delegate class
//

ArrayTableView::ArrayTableView(QObject *parent) : QStyledItemDelegate(parent) {}

ArrayTableView::ArrayTableView(QObject *parent, QSqlRelationalTableModel *model)
    : QStyledItemDelegate(parent), model_(model) {}

ArrayTableView::~ArrayTableView() {
  //  delete model_;
}

bool ArrayTableView::editorEvent(QEvent *e, QAbstractItemModel *model,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) {
  qDebug() << "\nHelp Event fired";
  if (!e || !model) return false;

  if (e->type() == QEvent::MouseButtonDblClick) {
    QString val = model->data(index, Qt::DisplayRole).toString();

    if (model_->record().fieldName(index.column()) == "Id") {
      QString Id = model_->record(index.row()).field("Id").value().toString();
      if (Id.length() > 0) {
        QString tbl_new = "EdUnitLeads";
        QString tbl_current = model_->tableName();
        QSqlRelationalTableModel *tmp = new QSqlRelationalTableModel(model);
        tmp->setTable(tbl_new);
        QString filter = "LeadId=" + Id;
        tmp->setFilter(filter);
        tmp->select();
        QDialog *popup = new TableWidget(nullptr, tmp);
        popup->show();
      }
    }
    if (val == "...") {
      QString tbl_new = model_->record().fieldName(index.column());
      QString Id = model_->record(index.row()).field("Id").value().toString();
      QSqlRelationalTableModel *tmp = new QSqlRelationalTableModel(model);
      QString tbl_current = model_->tableName();
      tmp->setTable(tbl_new);
      QString filter;
      if (tbl_new == "Actions")
        filter = "LeadId=" + Id;
      else
        filter = tbl_current + "_Id=" + Id;
      //      qDebug() << "FILTER = " << filter;
      tmp->setFilter(filter);
      tmp->select();
      QDialog *popup = new TableWidget(nullptr, tmp);
      popup->show();
    }

    return true;
  }
  return QStyledItemDelegate::editorEvent(e, model, option, index);
}
