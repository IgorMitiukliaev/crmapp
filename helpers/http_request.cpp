#include "http_request.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject(parent),
      manager_(new QNetworkAccessManager(this)),
      reply_(nullptr),
      data_buffer_(new QByteArray) {
  //  connect(manager_, &QNetworkAccessManager::finished, this,
  //          &HttpRequest::DataReadFinished);
}

HttpRequest::~HttpRequest() {}

QByteArray HttpRequest::MakeHTTPRequest(QString &req_type, QUrlQuery &param) {
  QUrl url;
  data_buffer_->clear();
  url.setScheme("https");
  url.setHost("movavischool.t8s.ru");
  url.setPath("/Api/V2/" + req_type);
  url.setQuery(param.toString());
  qDebug() << "\n\nParams = " << param.toString();
  QNetworkRequest request;
  request.setUrl(url);
  qDebug() << "manager";
  QNetworkAccessManager manager;
  QNetworkReply *reply = manager.get(request);
  auto future = QtFuture::connect(reply, &QNetworkReply::finished);
  qDebug() << "future "
           << future
                  .then(QtFuture::Launch::Async,
                        [&reply] { return reply->readAll(); })
                  .result();

  return future
      .then(QtFuture::Launch::Async, [&reply] { return reply->readAll(); })
      .result();
}

auto HttpRequest::ReadData() -> QByteArray * { return data_buffer_; }

void HttpRequest::DataReadFinished() {
  if (reply_->error()) {
    //    qDebug() << "Error : " << reply_->errorString();
  } else {
    data_buffer_->append(reply_->readAll());
    std::cout << data_buffer_->toStdString() << std::endl;
  }
}
