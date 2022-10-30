#include "sqlrelationaltablemodel.h"

SqlRelationalTableModel::SqlRelationalTableModel() {
  db_ = QSqlDatabase::addDatabase("QSQLITE");
  db_.setDatabaseName(":memory:");
  db_.setHostName("host");
  db_.setDatabaseName("dbcrm");
  db_.setUserName("root");
  db_.setPassword("root");
  qDebug() << db_.open();
  model_ = new QSqlRelationalTableModel(nullptr, db_);
  model_->setEditStrategy(QSqlTableModel::OnManualSubmit);

  QSqlQuery query;
}

SqlRelationalTableModel::~SqlRelationalTableModel() {
  delete model_;
  if (db_.isOpen()) db_.close();
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

bool SqlRelationalTableModel::SetLeadsTbl(const QJsonArray &data) {
  int max_len = 0;
  QSqlQuery query;
  QStringList labels = DefineLabels(data);
  QString request =
      "create table if not exists leads ("
      "Id integer PRIMARY KEY,";
  for (QString label : labels)
    if (label != "Id") request.append(label + " VARCHAR[1000] default NULL,");
  request.truncate(request.length() - 1);
  request.append(");");
  qDebug() << request;
  qDebug() << "Table created: " << query.exec(request);
  qDebug() << "Table truncated: " << query.exec("DELETE FROM leads;");

  request = "INSERT INTO leads (";
  for (QString label : labels) request.append(label + ", ");
  request.truncate(request.length() - 2);
  request.append(") VALUES (");
  for (QString label : labels) request.append(":" + label + ", ");
  request.truncate(request.length() - 2);
  request.append(");");
  query.prepare(request);
  qDebug() << request;

  for (const QJsonValue &val : data) {
    if (val.type() == QJsonValue::Object) {
      QJsonObject val_obj = val.toObject();
      QStringList keys = val_obj.keys();
      query.bindValue(":Id", val["Id"].toInt());
      for (QString key : keys) {
        if (key != "Id") query.bindValue(QString(":" + key), convert(key, val));
        qDebug() << "key = " << key << ", val = " << convert(key, val);
        max_len =
            std::max(max_len, static_cast<int>(convert(key, val).length()));
      }
      qDebug() << "Data inserted: " << query.exec();
    }
  }

  model_->setTable("leads");
  model_->select();
  qDebug() << "Sample record "
           << model_->record(0).value("FirstName").toString();
  qDebug() << "Tables available " << db_.tables();
  qDebug() << "\n\nROWS from model = " << model_->rowCount();
  qDebug() << "\n\nMax length = " << max_len;
  return true;
}

bool SqlRelationalTableModel::SetTable(const QString tbl_name,
                                       const QJsonArray &data, bool show) {
  bool res = true;
  QSqlQuery query;
  query.exec("DROP TABLE IF EXISTS " + tbl_name + ";");
  QStringList labels = DefineLabels(data);
  QString request = "create table if not exists " + tbl_name +
                    " ("
                    "Id integer PRIMARY KEY,";
  for (QString label : labels)
    if (label != "Id") request.append(label + " VARCHAR[1000] default NULL,");
  request.truncate(request.length() - 1);
  request.append(");");
  res = query.exec(request) && res;
  qDebug() << "Table " << tbl_name << " created: " << res;

  query.exec("DELETE FROM " + tbl_name + ";");

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
      for (QString key : keys) {
        if (key != "Id") query.bindValue(QString(":" + key), convert(key, val));
      }
      res = query.exec() && res;
    }
  }
  if (show) {
    model_->setTable(tbl_name);
    model_->select();
  }
  qDebug() << "Tables available " << db_.tables();
  return res;
}

auto SqlRelationalTableModel::convert(QString key, const QJsonValue &obj)
    -> QString {
  QString value;
  if (obj[key].type() == QJsonValue::Double)
    value = QString::number(obj[key].toDouble());
  else if (obj[key].toString().length() == 0)
    value = "NULL";
  else
    value = obj[key].toString();
  return value;
}
