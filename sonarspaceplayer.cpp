#include "sonarspaceplayer.h"

SonarSpacePlayer::SonarSpacePlayer()
    : silence(true),
      playing(false),
      timeToChangeState(oneSignalDuration)

{}

void SonarSpacePlayer::startPlaying() {
  if (!playing) {
    GenericSpacePlayer::startPlaying();
    silence = false;
    playing = true;
  }
}

void SonarSpacePlayer::stopPlaying() {
  if (playing) {
    GenericSpacePlayer::stopPlaying();
    silence = true;
    playing = false;
  }
}

void SonarSpacePlayer::updateTime(float duration) {
  if (playing) {
    timeToChangeState -= duration;
    if (timeToChangeState <= 0) {
      if (!silence) {
        GenericSpacePlayer::stopPlaying();
        timeToChangeState =
            listenerPos().distance(sonifiedPointPos()) * timeDistanceCoeff;
        silence = true;
      } else {
        GenericSpacePlayer::startPlaying();
        timeToChangeState = oneSignalDuration;
        silence = false;
      }
    }
  }
  GenericSpacePlayer::updateGains(duration);
}

std::string SonarSpacePlayer::getName() const noexcept {
  return std::string("Sonar");
}
