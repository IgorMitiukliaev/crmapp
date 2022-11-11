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
                                          QMap<QString, QString> *extra_fields,
                                          bool show) {
  bool res = true;
  QSqlQuery query;
  QStringList labels = DefineLabels(data);
  if (extra_fields != nullptr) {
    QMap<QString, QString>::ConstIterator it = extra_fields->begin();
    while (it != extra_fields->end()) {
      labels.append(it.key());
      ++it;
    }
  }

  QString request = "create table if not exists " + tbl_name + " (";
  for (QString label : labels)
    request.append(label + " VARCHAR[256] default NULL,");
  request.truncate(request.length() - 1);
  request.append(");");
  res = query.exec(request) && res;

  model_->setTable(tbl_name);
  qDebug() << "Table " << tbl_name << " created: " << res;

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
      //      query.bindValue(":Id", QString::number(val["Id"].toInt()));
      // clear all bindings
      for (QString key : labels) {
        if (key != "Id") {
          query.bindValue(QString(":" + key), "");
        } else {
          query.bindValue(":Id", QString::number(val["Id"].toInt()));
          // add new ids to table_id_set
          if (!table_id_set.contains(tbl_name))
            table_id_set.insert(tbl_name, QSet<QString>{});
          table_id_set[tbl_name].insert(QString::number(val["Id"].toInt()));
        }
      }
      // bind new values to object data
      for (QString key : keys) {
        if (key != "Id") query.bindValue(QString(":" + key), convert(key, val));
        if (convert(key, val) == "...") {
          QMap<QString, QString> ef{};
          ef.insert(tbl_name + "_Id", QString::number(val_obj["Id"].toInt()));
          CreateTable(key, val_obj[key].toArray(), &ef, false);
        }
      }
      // bind to extra fields
      if (extra_fields != nullptr) {
//        QSqlQuery query;
        QMap<QString, QString>::ConstIterator it = extra_fields->begin();
        while (it != extra_fields->end()) {
          query.bindValue(QString(":" + it.key()), it.value());
          ++it;
        }
      }
      res = query.exec() && res;
      //
    }
  }

  if (show) {
    model_->setTable(tbl_name);
    model_->select();
  }
  qDebug() << "Tables available " << db_.tables();
//  emit createTableFinished(tbl_name);
  return res;
}

auto SqlRelationalTableModel::GetValuesFromTable(QString table, QString field)
    -> QSet<QString> {
  QSet<QString> res;
  QSqlQuery query;
  query.exec("SELECT * FROM " + table + ";");
  while (query.next()) {
    QString id = query.value(field).toString();
    if (id.length() > 0) res.insert(id);
  }
  return res;
}

void SqlRelationalTableModel::SelectTable(QString table) {
  model_->setTable(table);
  model_->select();
}

auto SqlRelationalTableModel::convert(QString key, const QJsonValue &obj)
    -> QString {
  QString value;
  if (obj[key].type() == QJsonValue::Double)
    value = QString::number(obj[key].toInt());
  else if (obj[key].type() == QJsonValue::Array &&
           obj[key].toArray().size() > 0)
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

auto SqlRelationalTableModel::GetIDSet(QString tbl_name) -> QSet<QString> {
  return table_id_set[tbl_name];
}

QMap<QString, QString> SqlRelationalTableModel::GetLeadsStats() {
  QMap<QString, QString> res{};
    QSqlQuery query;
    query.exec("SELECT COUNT(DISTINCT Id) FROM Leads;");
    query.first();
    res.insert("Leads", query.value(0).toString());
    qDebug() << res;

  QSqlQuery query2;
  query2.exec(
      "SELECT COUNT(DISTINCT Id) FROM Leads INNER JOIN EdUnitLeads ON Leads.Id "
      "= EdUnitLeads.LeadId;");
  query2.first();
  res.insert("EdUnitLeads", query2.value(0).toString());
  qDebug() << res;
  return res;
}

auto SqlRelationalTableModel::ClearDb() -> void {
  QStringList tables = db_.tables();
  QSqlQuery query;
  for (QString table : tables) {
    table_id_set[table].clear();
    query.exec("DROP TABLE IF EXISTS " + table + ";");
    query.exec();
  }
}
