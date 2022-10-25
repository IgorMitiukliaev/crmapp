#include "http_request.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject(parent),
      manager_(new QNetworkAccessManager()),
      reply_(nullptr),
      buffer_(new QByteArray) {
  //    connect(manager_, &QNetworkAccessManager::finished, this,
  //            &HttpRequest::dataReadFinished);

  connect(manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(dataReadFinished(QNetworkReply *)));
}

HttpRequest::~HttpRequest() {
  delete manager_;
  delete buffer_;
}

auto HttpRequest::GetData(QString &req_name, const QUrlQuery &params) -> void {
  QUrl url;
  url.setScheme("https");
  url.setHost("movavischool.t8s.ru");
  url.setPath(QString::fromStdString("/Api/V2/") + req_name);
  url.setQuery(params.toString());
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  qDebug() << url;
  reply_ = manager_->post(request, url.toEncoded());
}

auto HttpRequest::dataReadFinished(QNetworkReply *r) -> void {
  qDebug() << r;
  buffer_->append(r->readAll());
  if (r->error()) {
    qDebug() << r->errorString();
  } else {
    qDebug() << QString(*buffer_);
  }
  r->deleteLater();
};
