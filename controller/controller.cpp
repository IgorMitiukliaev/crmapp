#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject(parent), request_(new HttpRequest) {
  connect(request_, &HttpRequest::dataReady, this, &Controller::DispatchData);
}

Controller::~Controller() { delete request_; };

auto Controller::Init() -> void {
  FileUtility f;
  std::string path = "/home/igor/Movavi/CRM/keys/keys.csv";
  keys_ = f.GetKeys(path);
  //  for (auto el : keys_) {
  //    std::cout << std::setw(40) << el.first << "  |  " << el.second <<
  //    std::endl;
  //  }
}

auto Controller::GetLeads(QUrlQuery params) -> void {
  QString path = "GetLeads";
  std::string key = keys_.at(path.toStdString());
  params.addQueryItem("authkey",
                      QUrl::toPercentEncoding(QString::fromStdString(key)));
  request_->MakeHTTPRequest(path, params);
};

auto Controller::GetHistoryModifyLeadStatus() -> void {
  QUrlQuery params;
  params.addQueryItem("dateTimeFrom", "2022-09-05");
  params.addQueryItem("dateTimeTo", "2022-09-15");
  QString path = "GetHistoryModifyLeadStatus";
  std::string key = keys_.at(path.toStdString());
  params.addQueryItem("authkey",
                      QUrl::toPercentEncoding(QString::fromStdString(key)));
  request_->MakeHTTPRequest(path, params);
};

auto Controller::GetOffices() -> void {
  QUrlQuery params;
  QString path = "GetOffices";
  std::string key = keys_.at(path.toStdString());
  params.addQueryItem("authkey",
                      QUrl::toPercentEncoding(QString::fromStdString(key)));
  request_->MakeHTTPRequest(path, params);
};

auto Controller::ExportData() -> void {
  QString path = "./out.json";
  QFile file(path);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream iStream(&file);
    iStream.setEncoding(QStringConverter::Utf8);
    iStream << *(request_->ReadData());
    file.close();
  } else {
    std::cout << "file open failed: " << path.toStdString() << std::endl;
  }
}

void Controller::DispatchData() {
  ExportData();
  emit dataReady();
}

auto Controller::GetJsonData() -> QJsonArray {
  QByteArray *raw_data = request_->ReadData();
  QJsonDocument json_data = QJsonDocument::fromJson(*raw_data);
  QJsonObject rootObject = json_data.object();
  return rootObject.value("Leads").toArray();
};
