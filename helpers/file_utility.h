#ifndef FILE_UTILITY_H_
#define FILE_UTILITY_H_

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>



class FileUtility {
 public:
  FileUtility() = default;
  ~FileUtility() = default;
  auto GetKeys(const std::string& path) -> std::map<std::string, std::string>;
};

#endif  // FILE_UTILITY_H_
