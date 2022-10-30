#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject(parent),
      request_(new HttpRequest),
      model_sqlr_(new SqlRelationalTableModel) {
  connect(request_, &HttpRequest::dataReady, this, &Controller::DispatchData);
}

Controller::~Controller() {
  delete request_;
  delete model_sqlr_;
};

auto Controller::Init() -> void {
  FileUtility f;
  std::string path = "/home/igor/Movavi/CRM/keys/keys.csv";
  keys_ = f.GetKeys(path);
}

auto Controller::GetDataFromApi(QString path, QUrlQuery params) -> void {
  params.addQueryItem("authkey", GetKey(path));
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
  ExportLeadsToModel();
}

auto Controller::GetKey(QString path) -> QByteArray {
  std::string key = keys_.at(path.toStdString());
  return QUrl::toPercentEncoding(QString::fromStdString(key));
}

void Controller::ExportLeadsToModel() {
  QJsonArray data = GetJsonData();
  model_sqlr_->SetTable("leads", data);
}

auto Controller::GetJsonData() -> QJsonArray {
  QByteArray *raw_data = request_->ReadData();
  QJsonDocument json_data = QJsonDocument::fromJson(*raw_data);
  QJsonObject rootObject = json_data.object();
  return rootObject.value("Leads").toArray();
}

bool Controller::GetGetHistoryModifyLeadStatus(QUrlQuery params) {
  QString path = "GetLeads";
  params.addQueryItem("authkey", GetKey(path));
  request_->MakeHTTPRequest(path, params);
  QJsonArray data = GetJsonData();
  model_sqlr_->SetTable("leads", data, false);

  path = "GetHistoryModifyLeadStatus";
  params.addQueryItem("authkey", GetKey(path));
  request_->MakeHTTPRequest(path, params);
  data = GetJsonData();
  model_sqlr_->SetTable("leads_history", data, false);
  return true;
};
