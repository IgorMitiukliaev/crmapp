#include "file_utility.h"

auto FileUtility::GetKeys(const std::string& path)
    -> std::map<std::string, std::string> {
  std::smatch m;
  std::ifstream filestream(path);
  std::map<std::string, std::string> res;
  if (filestream.is_open()) {
    std::string buffer;
    std::regex r(R"((^.+?);(.+)$)", std::regex::ECMAScript);
    while (!filestream.eof()) {
      std::getline(filestream, buffer);
      std::regex_search(buffer, m, r);
      if (m[2].length() > 0) res.insert(std::make_pair(m[1], m[2]));
    }
  }
  filestream.close();
  return res;
}
