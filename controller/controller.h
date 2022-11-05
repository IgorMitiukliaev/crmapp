#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QUrlQuery>
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
  inline auto ShareModel() -> QSqlTableModel* { return model_sqlr_->model_; };
  auto GetGetHistoryModifyLeadStatus(QUrlQuery params_) -> bool;

 public slots:
  auto ExportData() -> void;
  auto DispatchData() -> void;
  auto Dispatcher(QString) -> void;

 private:
  QString current_process_;
  QUrlQuery current_params_;
  QSet<QString> buffer_;
  std::map<std::string, std::string> keys_;
  HttpRequest* request_;
  SqlRelationalTableModel* model_sqlr_;
  auto GetKey(QString path) -> QByteArray;
  auto ExportLeadsToModel() -> void;
};

#endif  // CONTROLLER_H
