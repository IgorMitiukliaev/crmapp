#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent),
      request_(new HttpRequest),
      model_sqlr_(new SqlRelationalTableModel) {
  connect(request_, &HttpRequest::dataReady, this, &Controller::DispatchData);
//  connect(request_, &HttpRequest::next, this, &Controller::RunRoutine);
  connect(model_sqlr_, &SqlRelationalTableModel::next, this,
          &Controller::RunRoutine);
  connect(this, &Controller::next, this, &Controller::RunRoutine);
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
  current_params_.addQueryItem("authkey", GetKey(current_process_));

  std::function<void()> f = [this]() {
    qDebug() << "1st";
    request_->MakeHTTPRequest(current_process_, current_params_);
  };
  routine_[0] = f;

  f = [this]() {
    qDebug() << "3rd";
    id_set = model_sqlr_->GetIDSet("Leads");
    qDebug() << id_set;
    int i = 0;
    QList<int> list_id = routine_.keys();
    int max_ind = *std::max_element(list_id.begin(), list_id.end());
    for (const QString& id : id_set) {
      std::function<void()> ff = [=]() {
        QString name = "GetEdUnitLeads";
        QUrlQuery params;
        params.addQueryItem("authkey", GetKey(name));
        params.addQueryItem("LeadId", id);
        request_->MakeHTTPRequest(name, params);
      };
      routine_[max_ind + i++] = ff;
      ff = [=]() {
        QString name = "GetHistoryModifyLeadStatus";
        QUrlQuery params;
        params.addQueryItem("authkey", GetKey(name));
        params.addQueryItem("LeadId", id);
        request_->MakeHTTPRequest(name, params);
      };
      routine_[max_ind + i++] = ff;
    }
    emit next();
  };
  routine_[1] = f;

  emit next();
  return true;
}

void Controller::DispatchData() {
  ExportData();
  ExportDataToModel();
  model_sqlr_->SelectTable("Leads");
  emit next();
}

void Controller::ExportDataToModel() {
  QJsonArray data{};
  QString tbl_name;
  GetJsonData(tbl_name, data);
  model_sqlr_->CreateTable(tbl_name, data, nullptr, false);
}

void Controller::RunRoutine() {
  QList<int> list_id = routine_.keys();
  if (list_id.size() > 0) {
    int num = *std::min_element(list_id.begin(), list_id.end());
    if (routine_.contains(num) && routine_[num] != 0) {
      auto f = routine_[num];
      f();
      routine_.remove(num);
    }
  }
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
