#pragma once
#include <chrono>
#include <string>

class Duration : public std::chrono::milliseconds {
 public:
  Duration(const std::chrono::milliseconds& otherDuration =
               std::chrono::milliseconds(0));
  [[nodiscard]] std::string toString() const;
};
