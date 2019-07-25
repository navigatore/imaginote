#include "filenamestring.h"

FileNameString::FileNameString(const char *&cString) : std::string(cString) {}

void FileNameString::removeDirectoryPath() {
  auto lastSlashIndex = find_last_of("\\/");
  if (lastSlashIndex != std::string::npos) {
    erase(0, lastSlashIndex + 1);
  }
}

void FileNameString::removeExtension() {
  auto periodIdx = rfind('.');
  if (periodIdx != std::string::npos) {
    erase(periodIdx);
  }
}
