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
  SqlRelationalTableModel(QObject* parent = nullptr);
  ~SqlRelationalTableModel();
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  auto CreateTable(const QString tbl_name, const QJsonArray& data,
                   QMap<QString, int>* extra_fields = nullptr,
                   bool show = true) -> bool;
  auto GetHistoryModifyLeadStatus() -> QSet<QString>;
  QSqlRelationalTableModel* model_;
 signals:
  void createTableFinished(QString tbl_name);

 private:
  QSqlDatabase db_;
  bool state_;
  auto DefineLabels(const QJsonArray& data) -> QStringList;
  auto convert(QString key, const QJsonValue& val) -> QString;
  auto GetColumnIndex(QString const name) -> int;
};

#endif  // SQLRELATIONALTABLEMODEL_H
