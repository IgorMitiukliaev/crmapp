#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <iostream>

#include "constants.h"

class HttpRequest : public QObject {
  Q_OBJECT
 public:
  HttpRequest(QObject *parent = nullptr);
  ~HttpRequest();
  auto MakeHTTPRequest(QString &, QUrlQuery &) -> void;
  auto ReadData() -> QByteArray *;

 signals:
  void dataReady();

 public slots:
  void DataReadyRead();
  void DataReadFinished();

 private:
  QNetworkAccessManager *manager_;
  QNetworkReply *reply_;
  QByteArray *data_buffer_;
};
#endif  // HTTP_REQUEST_H
