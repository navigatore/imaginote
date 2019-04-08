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
  GenericSpacePlayer(unsigned int samplesLength = sample_rate * 1);
  virtual ~GenericSpacePlayer();

  static const unsigned int sample_rate = 44100;
  static constexpr float volumeMultiplierChangeStep = 0.1f;

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
  Coordinates2d listenerPos, sonifiedPointPos;

 private:
  void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);

  static void fadeIn(int16_t *buf, unsigned int buf_samples);
  static void fadeOut(int16_t *buf, unsigned int buf_samples);

  bool playing;
  unsigned int n;
  const unsigned int buf_samples_len;
  const unsigned int buf_size;

  float curTime;
  float volumeMultiplier;

  ALuint *src;
  ALuint *buf;
  std::vector<int16_t *> samples;
  ALCdevice *device;
  ALCcontext *context;
};

#endif  // GENERICSPACEPLAYER_H
