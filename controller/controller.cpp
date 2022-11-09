#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent),
      request_(new HttpRequest),
      model_sqlr_(new SqlRelationalTableModel) {
  connect(request_, &HttpRequest::dataReady, this, &Controller::DispatchData);
  connect(model_sqlr_, &SqlRelationalTableModel::createTableFinished, this,
          &Controller::Dispatcher);
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
  current_process_ = path;
  params.addQueryItem("authkey", GetKey(path));
  request_->MakeHTTPRequest(path, params);
};

bool Controller::GetFullLeadsData(QUrlQuery params) {
  current_process_ = "GetLeads";
  current_params_ = params;
  Dispatcher(current_process_);
  return true;
}

auto Controller::Dispatcher(QString name) -> void {
  if (current_process_ == "GetLeads") {
    if (name == "GetLeads") {
      QString path = "GetLeads";
      current_params_.addQueryItem("authkey", GetKey(path));
      request_->MakeHTTPRequest(path, current_params_);
    } else if (name == "Leads") {
      current_process_ = "GetHistoryModifyLeadStatus";
      current_params_.clear();
      current_params_.addQueryItem("authkey", GetKey(current_process_));
      model_sqlr_->GetValuesFromTable("Leads", "Id", buffer_);
    }
  } else if (current_process_ == "GetHistoryModifyLeadStatus") {
    if ((name == "Actions" || name == "GetValuesFromTable_Leads_Id")) {
      qDebug() << buffer_;
      qDebug() << buffer_.size();

      QSet<QString> buffer_tmp = buffer_;
      QSet<QString>::Iterator it = buffer_tmp.begin();
      if (it->length() > 0) {
        current_params_.addQueryItem("LeadId", *it);
        buffer_tmp.erase(it);
        qDebug() << "\nBUFFER = " << buffer_tmp;
        buffer_ = buffer_tmp;
        request_->MakeHTTPRequest(current_process_, current_params_);
      } else if (it->length() == 0) {
        current_process_ = "GetEdUnitLeads";
        current_params_.clear();
        model_sqlr_->GetValuesFromTable("Leads", "Id", buffer_);
      }
    }
  } else if (current_process_ == "GetEdUnitLeads") {
    if ((name == "EdUnitLeads" || name == "GetValuesFromTable_Leads_Id") &&
        buffer_.size() > 0) {
      current_params_.addQueryItem("authkey", GetKey(current_process_));
      QSet<QString> buffer_tmp = buffer_;
      QSet<QString>::Iterator it = buffer_tmp.begin();
      if (it->length() > 0) {
        current_params_.addQueryItem("LeadId", *it);
        buffer_tmp.erase(it);
        buffer_ = buffer_tmp;
        qDebug() << "\nBUFFER = " << buffer_;
        request_->MakeHTTPRequest(current_process_, current_params_);
      } else if (it->length() == 0) {
        model_sqlr_->SelectTable("Leads");
        //      current_process_ = "";
        //      current_params_.clear();
      }
    }
  }
}

void Controller::DispatchData() {
  ExportData();
  if (current_process_ == "GetLeads") {
    ExportLeadsToModel();
  } else {
    ExportDataToModel();
  }
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
