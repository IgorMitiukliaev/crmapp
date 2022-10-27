#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrlQuery>
#include <QFile>
#include <QTextStream>
#include <iomanip>

#include "helpers/file_utility.h"
#include "helpers/http_request.h"

class Controller : public QObject {
  Q_OBJECT
 public:
  Controller(QObject* parent = nullptr);
  ~Controller();
  auto Init() -> void;
  auto GetLeads(QUrlQuery params) -> void;
  auto GetOffices() -> void;
  auto GetHistoryModifyLeadStatus() -> void;

 public slots:
  auto ReadData() -> void;

 private:
  std::map<std::string, std::string> keys_;
  HttpRequest* request_;
};

#endif  // CONTROLLER_H
