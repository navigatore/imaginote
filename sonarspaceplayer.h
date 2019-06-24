#pragma once
#include "genericspaceplayer.h"

class SonarSpacePlayer : public GenericSpacePlayer {
 public:
  SonarSpacePlayer();

  virtual void startPlaying();
  virtual void stopPlaying();
  virtual void updateTime(float duration);

 private:
  constexpr static const float oneSignalDuration = 0.5f;
  constexpr static const float timeDistanceCoeff = 0.25f;

  bool silence;
  bool playing;
  float timeToChangeState;
};
