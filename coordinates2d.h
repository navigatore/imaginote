#pragma once
class Coordinates2d {
 public:
  explicit Coordinates2d(float x = 0, float y = 0);

  [[nodiscard]] float distance(const Coordinates2d& other) const;

  [[nodiscard]] const float& x() const;
  float& x();
  [[nodiscard]] const float& y() const;
  float& y();

 private:
  float _x, _y;
};
