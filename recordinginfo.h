#pragma once
#include <optional>

#include "angle.h"
#include "space.h"

class RecordingInfo {
 public:
  RecordingInfo() = default;

  class NotFullySetUp : public std::exception {};

  void setSpaceName(const std::string& spaceName);
  void setSpace(const Space& space);
  void setSonificationMethodName(const std::string& sonificationMethodName);
  void setAngleX(const Angle& angleX);
  void setMaxDistance(float maxDistance);

  [[nodiscard]] std::string getSpaceName() const;
  [[nodiscard]] Space getSpace() const;
  [[nodiscard]] std::string getSonificationMethodName() const;
  [[nodiscard]] Angle getAngleX() const;
  [[nodiscard]] float getMaxDistance() const;

 private:
  void fullySetUpTest() const;

  [[nodiscard]] bool fullySetUp() const noexcept;

  std::optional<std::string> spaceName;
  std::optional<Space> space;
  std::optional<std::string> sonificationMethodName;
  std::optional<Angle> angleX;
  std::optional<float> maxDistance;
};
