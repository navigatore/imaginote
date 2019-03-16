#ifndef SONARSPACEPLAYER_H
#define SONARSPACEPLAYER_H

#include "genericspaceplayer.h"

class SonarSpacePlayer : public GenericSpacePlayer {
 public:
  SonarSpacePlayer();

  virtual void startPlaying();
  virtual void stopPlaying();
  virtual void update(float duration);

 private:
  constexpr static const float oneSignalDuration = 0.5f;
  constexpr static const float timeDistanceCoeff = 0.25f;

  bool silence;
  bool playing;
  float timeToChangeState;
};

#endif  // SONARSPACEPLAYER_H
