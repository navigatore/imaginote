#pragma once
#include "genericspaceplayer.h"
class SimpleSpacePlayer : public GenericSpacePlayer {
 public:
  virtual std::string getName() const noexcept override;
};
