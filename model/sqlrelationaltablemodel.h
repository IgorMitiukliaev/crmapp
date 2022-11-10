#ifndef SQLRELATIONALTABLEMODEL_H
#define SQLRELATIONALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QEvent>
#include <QPluginLoader>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QStringList>
#include <QTableView>
#include <QTableWidgetItem>
#include <QtSql>

class SqlRelationalTableModel : public QObject {
  Q_OBJECT
 public:
  QSqlRelationalTableModel* model_;
  SqlRelationalTableModel(QObject* parent = nullptr);
  ~SqlRelationalTableModel();
  auto CreateTable(const QString tbl_name, const QJsonArray& data,
                   QMap<QString, QString>* extra_fields = nullptr,
                   bool show = true) -> bool;
  auto GetHistoryModifyLeadStatus() -> QSet<QString>;
  auto GetValuesFromTable(QString table, QString field) -> QSet<QString>;
  auto SelectTable(QString table) -> void;
  auto ClearDb() -> void;
  auto GetColumnIndex(QString const name) -> int;

 signals:
  void createTableFinished(QString tbl_name);

 private:
  QSqlDatabase db_;
  bool state_;
  auto DefineLabels(const QJsonArray& data) -> QStringList;
  auto convert(QString key, const QJsonValue& val) -> QString;
};

#endif  // SQLRELATIONALTABLEMODEL_H
