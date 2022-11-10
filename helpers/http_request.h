#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QFuture>
#include <QPromise>
#include <QtConcurrent>
#include <iostream>

#include "constants.h"

class HttpRequest : public QObject {
  Q_OBJECT
 public:
  HttpRequest(QObject *parent = nullptr);
  ~HttpRequest();
  QByteArray MakeHTTPRequest(QString &, QUrlQuery &);
  auto ReadData() -> QByteArray *;

 public slots:
  void DataReadFinished();

 private:
  QNetworkAccessManager *manager_;
  QNetworkReply *reply_;
  QByteArray *data_buffer_;
};
#endif  // HTTP_REQUEST_H
