#pragma once

#include <string>

class FileNameString : public std::string {
 public:
  FileNameString() = default;
  FileNameString(const char*& cString);

  void removeDirectoryPath();
  void removeExtension();
};
