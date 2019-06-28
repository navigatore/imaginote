#pragma once
#include <boost/serialization/binary_object.hpp>
#include <optional>
#include <vector>
#include "coordinates.h"
#include "segment.h"

class SimpleSpaceObject {
 public:
  SimpleSpaceObject(Coordinates crd = Coordinates(), unsigned int height = 0,
                    bool focus = false);

  class CornersNotSetUp : public std::exception {};

  bool operator==(const SimpleSpaceObject& other);
  bool operator!=(const SimpleSpaceObject& other);

  void setCorners(const SimpleSpaceObject* top, const SimpleSpaceObject* right,
                  const SimpleSpaceObject* bottom,
                  const SimpleSpaceObject* left);

  [[nodiscard]] std::vector<Coordinates> getCorners();

  [[nodiscard]] bool isInside(const Coordinates& point) const;
  [[nodiscard]] std::vector<Coordinates2d> getIntersectionPts(
      const Segment& segm) const;

  [[nodiscard]] const Coordinates& crds() const;
  Coordinates& crds();
  [[nodiscard]] const unsigned int& height() const;
  unsigned int& height();
  [[nodiscard]] const bool& focus() const;
  bool& focus();
  [[nodiscard]] const bool& visited() const;
  bool& visited();

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& _crds;
    archive& boost::serialization::make_binary_object(&corners,
                                                      sizeof(corners));
    archive& _height;
    archive& _focus;
    archive& _visited;
  }

  Coordinates _crds;
  std::optional<std::vector<Coordinates>> corners;
  unsigned int _height;
  bool _focus;
  bool _visited{};
};
