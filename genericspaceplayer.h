#ifndef GENERICSPACEPLAYER_H
#define GENERICSPACEPLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdlib>
#include <vector>
#include "angle.h"
#include "simplespaceobject.h"

class GenericSpacePlayer {
 public:
  GenericSpacePlayer();
  virtual ~GenericSpacePlayer();

  virtual void startPlaying();
  virtual void stopPlaying();
  virtual void setSonificationObject(const SimpleSpaceObject &obj);
  virtual void updateSonifiedPointPosition(const Coordinates2d &pos);
  virtual void updateListenerPosition(const Coordinates &pos,
                                      const Angle &angle);
  virtual void update(float duration);
  virtual void volumeUp();
  virtual void volumeDown();

  float getVolume() const;

 protected:
  static const unsigned int sample_rate = 44100;
  static const unsigned int buf_size = sample_rate * 2;
  static constexpr float volumeMultiplierChangeStep = 0.1f;
  static constexpr float fadeTime = 0.1f;
  static constexpr float baseFrequency = 440.0f;

  float getFrequency(unsigned int height);
  Coordinates2d listenerPos, sonifiedPointPos;

 private:
  void startPrimary();
  void stopPrimary();
  void startSource(unsigned int idx);
  void stopSource(unsigned int idx);

  void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);
  void setGains();

  bool playing;
  unsigned int primaryIdx;

  float curTime;
  float volumeMultiplier;
  float primaryGain, secondaryGain;
  float primaryFrequency;
  ALuint *src;
  ALuint *buf;
  std::vector<int16_t *> samples;
  ALCdevice *device;
  ALCcontext *context;
};

#endif  // GENERICSPACEPLAYER_H
