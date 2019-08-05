#pragma once
#include <optional>
#include "angle.h"
#include "space.h"

class RecordingInfo {
 public:
  RecordingInfo();

  class NotFullySetUp : public std::exception {};

  void setSpace(const Space& space);
  void setSonificationMethodName(const std::string& sonificationMethodName);
  void setAngleX(const Angle& angleX);
  void setMaxDistance(float maxDistance);

  [[nodiscard]] Space getSpace() const;
  [[nodiscard]] std::string getSonificationMethodName() const;
  [[nodiscard]] Angle getAngleX() const;
  [[nodiscard]] float getMaxDistance() const;

 private:
  void fullySetUpTest() const;

  [[nodiscard]] bool fullySetUp() const noexcept;

  std::optional<Space> space;
  std::optional<std::string> sonificationMethodName;
  std::optional<Angle> angleX;
  std::optional<float> maxDistance;
};
