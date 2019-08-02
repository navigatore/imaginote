#pragma once
#include "genericspaceplayer.h"
class NewSpacePlayer : public GenericSpacePlayer {
 public:
  virtual std::string getName() const noexcept override;
};
