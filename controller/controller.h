#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QUrlQuery>
#include <QtConcurrent>
#include <iomanip>

#include "helpers/file_utility.h"
#include "helpers/http_request.h"
#include "model/sqlrelationaltablemodel.h"

class Controller : public QObject {
  Q_OBJECT

 public:
  Controller(QObject* parent = nullptr);
  ~Controller();
  auto Init() -> void;
  auto GetDataFromApi(QString path, QUrlQuery params) -> void;
  auto GetJsonData(QString&, QJsonArray&) -> void;
  inline auto ShareModel() -> QSqlRelationalTableModel* {
    return model_sqlr_->model_;
  };
  auto GetFullLeadsData(QUrlQuery params_) -> bool;
  //  auto GetHistoryModifyLeadStatus() -> bool;
  auto ClearDb() -> void;

 signals:
  void next();

 public slots:
  auto ExportData() -> void;
  auto DispatchData() -> void;
  auto RunRoutine() -> void;

 private:
  QString current_process_;
  QUrlQuery current_params_;
  QMap<int, std::function<void()>> routine_;
  std::map<std::string, std::string> keys_;
  QSet<QString> id_set;
  HttpRequest* request_;
  SqlRelationalTableModel* model_sqlr_;
  auto GetKey(QString path) -> QByteArray;
  void ExportDataToModel();
};

#endif  // CONTROLLER_H
