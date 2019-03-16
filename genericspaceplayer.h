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

 protected:
  Coordinates2d listenerPos, sonifiedPointPos;

 private:
  static const unsigned int sample_rate = 44100;

  void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);

  bool playing;
  unsigned int n;
  const unsigned int buf_samples_len;
  const unsigned int buf_size;

  float curTime;

  ALuint *src;
  ALuint *buf;
  std::vector<int16_t *> samples;
  ALCdevice *device;
  ALCcontext *context;
};

#endif  // GENERICSPACEPLAYER_H
