#ifndef COORDINATES2D_H
#define COORDINATES2D_H

class Coordinates2d {
 public:
  Coordinates2d(float x = 0, float y = 0);

  [[nodiscard]] float distance(const Coordinates2d& other) const;

  [[nodiscard]] const float& x() const;
  float& x();
  [[nodiscard]] const float& y() const;
  float& y();

 private:
  float _x, _y;
};

#endif  // COORDINATES2D_H
