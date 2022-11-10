#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QUrlQuery>
#include <iomanip>
#include <QtConcurrent>
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
  auto ClearDb() -> void;
  static auto ExportData(QByteArray &&data) -> void;

 private:
  QString current_process_;
  QUrlQuery current_params_;
  QSet<QString> buffer_;
  std::map<std::string, std::string> keys_;
  HttpRequest* request_;
  SqlRelationalTableModel* model_sqlr_;
  auto GetKey(QString path) -> QByteArray;
  void ExportDataToModel();
  auto ExportLeadsToModel() -> void;
  static auto MakeHTTPRequest(QString, QUrlQuery) -> QByteArray;
};
#endif  // CONTROLLER_H
