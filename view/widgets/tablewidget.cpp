#include "tablewidget.h"

#include "ui_tablewidget.h"

TableWidget::TableWidget(QWidget *parent, QAbstractItemModel *model)
    : QDialog(parent), ui(new Ui::TableWidget) {
  ui->setupUi(this);
  ui->wg_tbl_view->setModel(model);
}

TableWidget::~TableWidget() { delete ui; }

void TableWidget::on_btn_OK_accepted() { accept(); }
