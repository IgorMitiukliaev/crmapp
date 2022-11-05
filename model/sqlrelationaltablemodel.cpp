#include "sqlrelationaltablemodel.h"

SqlRelationalTableModel::SqlRelationalTableModel(QObject *parent)
    : QObject(parent) {
  db_ = QSqlDatabase::addDatabase("QSQLITE");
  db_.setDatabaseName(":memory:");
  db_.setHostName("host");
  db_.setDatabaseName("dbcrm");
  db_.setUserName("root");
  db_.setPassword("root");
  qDebug() << db_.open();
  model_ = new QSqlRelationalTableModel(nullptr, db_);
  model_->setEditStrategy(QSqlTableModel::OnManualSubmit);
  //  model_->setHeaderData(0, Qt::Horizontal, "dsfdf", Qt::ToolTipRole);

  QSqlQuery query;
}

SqlRelationalTableModel::~SqlRelationalTableModel() {
  delete model_;
  QSqlQuery query;
  QStringList tables = db_.tables();
  QList<QString>::Iterator it = tables.begin();
  while (it != tables.end()) {
    query.exec("DROP TABLE IF EXISTS " + *it + ";");
    ++it;
  }
  if (db_.isOpen()) db_.close();
}

QVariant SqlRelationalTableModel::data(const QModelIndex &index,
                                       int role) const {
  qDebug() << "\nTOOLTIP \n";
  QVariant value = model_->data(index, role);
  switch (role) {
    case Qt::ToolTipRole:
      return model_->data(index);
  }
  return value;
}

auto SqlRelationalTableModel::DefineLabels(const QJsonArray &data)
    -> QStringList {
  QStringList headers{};
  for (const QJsonValue &val : data) {
    if (val.type() == QJsonValue::Object) {
      QStringList keys = val.toObject().keys();
      for (QString key : keys) headers.append(key);
    }
  }
  headers.removeDuplicates();
  return headers;
}

bool SqlRelationalTableModel::CreateTable(const QString tbl_name,
                                          const QJsonArray &data,
                                          QMap<QString, int> *extra_fields,
                                          bool show) {
  bool res = true;
  QSqlQuery query;
//  query.exec("DROP TABLE IF EXISTS " + tbl_name + ";");

  QStringList labels = DefineLabels(data);
  if (extra_fields != nullptr) {
    QMap<QString, int>::ConstIterator it = extra_fields->begin();
    while (it != extra_fields->end()) {
      labels.append(it.key());
      ++it;
    }
  }

  QString request = "create table if not exists " + tbl_name +
                    " ("
                    "Id integer,";

  for (QString label : labels)
    if (label != "Id") request.append(label + " VARCHAR[1000] default NULL,");
  request.truncate(request.length() - 1);
  request.append(");");
  res = query.exec(request) && res;

  model_->setTable(tbl_name);
  qDebug() << "Table " << tbl_name << " created: " << res;

//  query.exec("DELETE FROM " + tbl_name + ";");

  request = "INSERT INTO " + tbl_name + " (";
  for (QString label : labels) request.append(label + ", ");
  request.truncate(request.length() - 2);
  request.append(") VALUES (");
  for (QString label : labels) request.append(":" + label + ", ");
  request.truncate(request.length() - 2);
  request.append(");");
  query.prepare(request);

  for (const QJsonValue &val : data) {
    if (val.type() == QJsonValue::Object) {
      QJsonObject val_obj = val.toObject();
      QStringList keys = val_obj.keys();
      query.bindValue(":Id", val["Id"].toInt());
      // clear all bindings
      for (QString key : keys) {
        if (key != "Id") query.bindValue(QString(":" + key), "");
      }
      // bind new values to object data
      for (QString key : keys) {
        if (key != "Id") query.bindValue(QString(":" + key), convert(key, val));
        if (convert(key, val) == "...") {
          QMap<QString, int> ef{};
          ef.insert(tbl_name + "_Id", val_obj["Id"].toInt());
          CreateTable(key, val_obj[key].toArray(), &ef, false);
          //          qDebug() << "\n\nTHIS IS AN ARRAY\n"
          //                   << key << "at " << GetColumnIndex(key) << "\n"
          //                   << val_obj[key];
        }
      }
      // bind to extra fields
      if (extra_fields != nullptr) {
        QMap<QString, int>::ConstIterator it = extra_fields->begin();
        while (it != extra_fields->end()) {
          query.bindValue(QString(":" + it.key()), it.value());
          ++it;
        }
      }
      res = query.exec() && res;
      //
    }
  }

  //  model_->setTable(tbl_name);
  //  model_->select();
  //  int rows = model_->rowCount();
  //  int cols = model_->columnCount();
  //  for (int row = 0; row < rows; ++row) {
  //    for (int col = 0; col < cols; ++col) {
  //      QMap<int, QVariant> qmap{};
  //      qmap.insert(Qt::ToolTipRole,
  //      model_->record(row).value(col).toString());
  //      model_->setItemData(model_->index(row, col), qmap);
  //    }
  //  }

  if (show) {
    model_->setTable(tbl_name);
    model_->select();
  }
  qDebug() << "Tables available " << db_.tables();
  emit createTableFinished(tbl_name);
  return res;
}

auto SqlRelationalTableModel::GetHistoryModifyLeadStatus() -> QSet<QString> {
  qDebug() << "\n\nActions fired";
  QSet<QString> res{};
  QSqlQuery query;
  query.exec(
      "SELECT *"
      "FROM Leads"
      ";");
  while (query.next()) {
    QString id = query.value("StudentClientId").toString();
    if (id.length() > 0) res.insert(id);
  }
  emit createTableFinished("StudentClientIdList");
  return res;
}

auto SqlRelationalTableModel::convert(QString key, const QJsonValue &obj)
    -> QString {
  QString value;
  if (obj[key].type() == QJsonValue::Double)
    value = QString::number(obj[key].toDouble());
  else if (obj[key].type() == QJsonValue::Array)
    value = "...";
  else if (obj[key].toString().length() == 0)
    value = "";
  else
    value = obj[key].toString();
  return value;
}

auto SqlRelationalTableModel::GetColumnIndex(QString const name) -> int {
  int i = 0;
  while (i < model_->columnCount() && model_->record(0).fieldName(i) != name) {
    i++;
  }
  return i;
}
