#ifndef SQLRELATIONALTABLEMODEL_H
#define SQLRELATIONALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QPluginLoader>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QStringList>
#include <QTableView>
#include <QtSql>

class SqlRelationalTableModel {
 public:
  QSqlRelationalTableModel* model_;
  SqlRelationalTableModel();
  ~SqlRelationalTableModel();
  auto SetLeadsTbl(const QJsonArray& data) -> bool;
  auto SetTable(const QString tbl_name, const QJsonArray& data, bool show = true) -> bool;

 private:
  QSqlDatabase db_;
  bool state_;
  auto DefineLabels(const QJsonArray& data) -> QStringList;
  auto convert(QString key, const QJsonValue& val) -> QString;
};

#endif  // SQLRELATIONALTABLEMODEL_H
