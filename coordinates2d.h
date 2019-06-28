#pragma once
#include <boost/archive/text_oarchive.hpp>
class Coordinates2d {
 public:
  explicit Coordinates2d(float x = 0, float y = 0);

  [[nodiscard]] float distance(const Coordinates2d& other) const;

  [[nodiscard]] const float& x() const;
  float& x();
  [[nodiscard]] const float& y() const;
  float& y();

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& _x;
    archive& _y;
  }

  float _x, _y;
};
