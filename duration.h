#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <chrono>
#include <string>

class Duration : public std::chrono::milliseconds {
 public:
  Duration(const std::chrono::milliseconds& otherDuration =
               std::chrono::milliseconds(0));
  [[nodiscard]] std::string toString() const;

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& boost::serialization::make_binary_object(this, sizeof(*this));
  }
};
