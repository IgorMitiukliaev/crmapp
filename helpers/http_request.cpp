#include "http_request.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject(parent),
      manager_(new QNetworkAccessManager(this)),
      reply_(nullptr),
      data_buffer_(new QByteArray) {
  connect(manager_, &QNetworkAccessManager::finished, this,
          &HttpRequest::DataReadFinished);
}

HttpRequest::~HttpRequest() {}

auto HttpRequest::MakeHTTPRequest(QString &req_type, QUrlQuery &param) -> void {
  QUrl url;
  data_buffer_->clear();
  url.setScheme("https");
  url.setHost("movavischool.t8s.ru");
  url.setPath("/Api/V2/" + req_type);
  url.setQuery(param.toString());
  qDebug() << "\n\nParams = " << param.toString();
  QNetworkRequest request;
  request.setUrl(url);
  reply_ = manager_->get(request);
}

auto HttpRequest::ReadData() -> QByteArray * { return data_buffer_; }

void HttpRequest::DataReadFinished() {
  if (reply_->error()) {
    //    qDebug() << "Error : " << reply_->errorString();
  } else {
    data_buffer_->append(reply_->readAll());
    std::cout << data_buffer_->toStdString() << std::endl;
    emit dataReady();
  }
}
