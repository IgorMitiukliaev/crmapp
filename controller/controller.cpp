#include "controller.h"

auto Controller::Init() -> void {
  FileUtility f;
  std::string path = "/home/igor/Movavi/CRM/keys/keys.csv";
  keys_ = f.GetKeys(path);
  for (auto el : keys_) {
    std::cout << std::setw(40) << el.first << "  |  " << el.second << std::endl;
  }
}

auto Controller::GetLeads() -> void {
  HttpRequest request;
  QUrlQuery params;
  params.addQueryItem("createdFrom","2022-08-05");
  params.addQueryItem("createdTo", "2022-09-25");
  QString path = "GetLeads";
  std::string key = keys_.at(path.toStdString());
  params.addQueryItem("authkey",
                      QUrl::toPercentEncoding(QString::fromStdString(key)));
  request.GetData(path, params);
};

auto Controller::GetOffices() -> void {
  HttpRequest request;
  QUrlQuery params;
  QString path = "GetOffices";
  std::string key = keys_.at(path.toStdString());
  params.addQueryItem("authkey",
                      QString::fromStdString(key));
  request.GetData(path, params);
};
