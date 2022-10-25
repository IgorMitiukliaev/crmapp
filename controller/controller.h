#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QUrlQuery>
#include <iomanip>

#include "helpers/file_utility.h"
#include "helpers/http_request.h"

class Controller {
 public:
  Controller() = default;
  ~Controller() = default;
  auto Init() -> void;
  auto GetLeads() -> void;
  auto GetOffices() -> void;

 private:
  std::map<std::string, std::string> keys_;
};

#endif  // CONTROLLER_H
