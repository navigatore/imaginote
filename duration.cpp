#include "duration.h"

Duration::Duration(const std::chrono::milliseconds &otherDuration)
    : std::chrono::milliseconds(otherDuration) {}

std::string Duration::toString() const {
  using namespace std::chrono_literals;

  std::string builder;

  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(*this);
  if (seconds != 0s) {
    builder += std::to_string(seconds.count()) + " s  ";
  }
  auto milliseconds = *this - seconds;
  builder += std::to_string(milliseconds.count()) + " ms";

  return builder;
}
