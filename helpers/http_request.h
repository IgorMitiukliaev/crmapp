#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <QUrlQuery>

#include "constants.h"

class HttpRequest : public QObject {
  Q_OBJECT
 public:
  HttpRequest(QObject* parent = nullptr);
  ~HttpRequest();

  auto GetData(QString&, const QUrlQuery&) -> void;

 private:
  QNetworkAccessManager* manager_;
  QNetworkReply* reply_;
  QByteArray* buffer_;

 private slots:
  auto dataReadFinished(QNetworkReply*) -> void;
};

#endif  // HTTP_REQUEST_H
