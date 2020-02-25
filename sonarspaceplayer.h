#pragma once
#include "genericspaceplayer.h"

class SonarSpacePlayer : public GenericSpacePlayer {
 public:
  SonarSpacePlayer();

  virtual void startPlaying() override;
  virtual void stopPlaying() override;
  virtual void updateTime(float duration) override;

  virtual std::string getName() const noexcept override;

 private:
  constexpr static const float oneSignalDuration = 0.5f;
  constexpr static const float timeDistanceCoeff = 0.25f;

  bool silence;
  bool playing;
  float timeToChangeState;
};
