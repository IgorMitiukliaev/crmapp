#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent),
      request_(new HttpRequest),
      model_sqlr_(new SqlRelationalTableModel) {}

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
  current_process_ = path;
  params.addQueryItem("authkey", GetKey(path));
  request_->MakeHTTPRequest(path, params);
};

auto Controller::MakeHTTPRequest(QString path, QUrlQuery params)
    -> QByteArray {
  HttpRequest request;
  return request.MakeHTTPRequest(path, params);
}

bool Controller::GetFullLeadsData(QUrlQuery params) {
  QUrlQuery current_params = params;
  QString path = "GetLeads";
  current_params.addQueryItem("authkey", GetKey(path));
  qDebug() << "GetLeads";

  QFuture<void> future =
      QtConcurrent::run(MakeHTTPRequest, path, current_params).then(ExportData);
  //          .then([this]() {
  //            qDebug() << "ExportData";
  //            return ExportData();
  //          })
  //          .then([this]() {
  //            qDebug() << "ExportDataToModel";
  //            return ExportDataToModel();
  //          })
  //          .then([this]() {
  //            qDebug() << "GetValuesFromTable";
  //            return model_sqlr_->GetValuesFromTable("Leads", "Id", buffer_);
  //          })
  //          .then([this]() {
  //            qDebug() << buffer_;
  //            qDebug() << buffer_.size();
  //            return true;
  //          });
  //  future.result();

  //  path = "GetHistoryModifyLeadStatus";
  //  current_params.clear();
  //  current_params.addQueryItem("authkey", GetKey(path));

  //  QSet<QString>::Iterator it = buffer_.begin();
  //  while (it != buffer_.end()) {
  //    if (it->length() > 0) {
  //      current_params.addQueryItem("LeadId", *it);
  //      request_->MakeHTTPRequest(path, current_params)
  //          .then([this]() { ExportData(); })
  //          .then([this]() { ExportDataToModel(); });
  //    }
  //    ++it;
  //  }

  //  path = "GetEdUnitLeads";
  //  current_params.clear();
  //  current_params.addQueryItem("authkey", GetKey(path));
  //  it = buffer_.begin();
  //  while (it != buffer_.end()) {
  //    if (it->length() > 0) {
  //      current_params.addQueryItem("LeadId", *it);
  //      request_->MakeHTTPRequest(path, current_params)
  //          .then([this]() { ExportData(); })
  //          .then([this]() { ExportDataToModel(); });
  //    }
  //    ++it;
  //  }
  return true;
}

void Controller::ExportDataToModel() {
  QJsonArray data{};
  QString tbl_name;
  GetJsonData(tbl_name, data);
  model_sqlr_->CreateTable(tbl_name, data, nullptr, false);
}

void Controller::ExportLeadsToModel() {
  QJsonArray data{};
  QString tbl_name;
  QMap<QString, QString> extra_fields;
  extra_fields.insert("Actions", "...");
  GetJsonData(tbl_name, data);
  model_sqlr_->CreateTable(tbl_name, data, &extra_fields, false);
  model_sqlr_->SelectTable("Leads");
}

auto Controller::ExportData(QByteArray&& data) -> void {
  qDebug() << "ExportData_IN \n" << data;
  QString path = "./out.json";
  QFile file(path);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream iStream(&file);
    iStream.setEncoding(QStringConverter::Utf8);
    iStream << data;
    file.close();
  } else {
    std::cout << "file open failed: " << path.toStdString() << std::endl;
  }
}

auto Controller::GetKey(QString path) -> QByteArray {
  std::string key = keys_.at(path.toStdString());
  return QUrl::toPercentEncoding(QString::fromStdString(key));
}

auto Controller::GetJsonData(QString& table_name, QJsonArray& data) -> void {
  QByteArray* raw_data = request_->ReadData();
  QJsonDocument json_data = QJsonDocument::fromJson(*raw_data);
  QJsonObject rootObject = json_data.object();
  QStringList keys = rootObject.keys();
  table_name = keys.isEmpty() ? "table" : keys.front();
  data = rootObject.value(table_name).toArray();
}

void Controller::ClearDb() { model_sqlr_->ClearDb(); }

// bool Controller::GetHistoryModifyLeadStatus() {
//   current_process_ = "GetHistoryModifyLeadStatus";
//   QUrlQuery params{};
//   params.addQueryItem("authkey", GetKey(current_process_));
//   request_->MakeHTTPRequest(current_process_, params);
//   return true;
// }
