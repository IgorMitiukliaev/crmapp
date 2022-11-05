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
  QJsonArray data{};
  QString tbl_name;
  GetJsonData(tbl_name, data);
  model_sqlr_->CreateTable(tbl_name, data, nullptr, true);
}

auto Controller::GetJsonData(QString& table_name, QJsonArray& data) -> void {
  QByteArray* raw_data = request_->ReadData();
  QJsonDocument json_data = QJsonDocument::fromJson(*raw_data);
  QJsonObject rootObject = json_data.object();
  QStringList keys = rootObject.keys();
  table_name = keys.isEmpty() ? "table" : keys.front();
  data = rootObject.value(table_name).toArray();
}

bool Controller::GetGetHistoryModifyLeadStatus(QUrlQuery params) {
  current_process_ = "GetGetHistoryModifyLeadStatus";
  current_params_ = params;
  Dispatcher(current_process_);
  return true;
}

auto Controller::Dispatcher(QString name) -> void {
  if (name == "GetGetHistoryModifyLeadStatus" &&
      current_process_ == "GetGetHistoryModifyLeadStatus") {
    qDebug() << "\n\nDispatcher fired with " << name;
    QString path = "GetLeads";
    QUrlQuery params_tmp = current_params_;
    params_tmp.addQueryItem("authkey", GetKey(path));
    params_tmp.removeQueryItem("dateTimeFrom");
    params_tmp.removeQueryItem("dateTimeTo");
    params_tmp.removeQueryItem("LeadId");
    request_->MakeHTTPRequest(path, params_tmp);

  } else if (name == "Leads" &&
             current_process_ == "GetGetHistoryModifyLeadStatus") {
    qDebug() << "\n\nDispatcher fired with " << name;
    QUrlQuery params_tmp = current_params_;
    params_tmp.removeQueryItem("Id");
    params_tmp.removeQueryItem("addressDateFrom");
    params_tmp.removeQueryItem("addressDateTo");
    params_tmp.removeQueryItem("createdFrom");
    params_tmp.removeQueryItem("createdTo");
    params_tmp.removeQueryItem("attached");
    QString path = "GetHistoryModifyLeadStatus";
    params_tmp.addQueryItem("authkey", GetKey(path));
    request_->MakeHTTPRequest(path, params_tmp);
  } else if (name == "Actions" &&
             current_process_ == "GetGetHistoryModifyLeadStatus") {
    qDebug() << "\n\nDispatcher fired with " << name;
    buffer_ = model_sqlr_->GetHistoryModifyLeadStatus();

  } else if (name == "StudentClientIdList" &&
             current_process_ == "GetGetHistoryModifyLeadStatus") {
    qDebug() << "\n\nDispatcher fired with " << name;
    QString path = "GetEdUnitStudents";
    current_params_.clear();
    current_params_.addQueryItem("authkey", GetKey(path));
    QSet<QString>::Iterator it = buffer_.begin();
    while (it != buffer_.end()) {
      qDebug() << *it;
      ++it;
    }
    current_process_ = "";
    current_params_.clear();
  }
}
