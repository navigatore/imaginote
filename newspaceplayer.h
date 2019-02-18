#ifndef NEWSPACEPLAYER_H
#define NEWSPACEPLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdlib>
#include <vector>
#include "angle.h"
#include "simplespaceobject.h"

class NewSpacePlayer {
 public:
  NewSpacePlayer();
  ~NewSpacePlayer();

  void sonificateObject(SimpleSpaceObject obj);
  void updateListenerPosition(Coordinates pos, Angle angle);
  void stopPlaying();

 private:
  static const unsigned int sample_rate = 44100;

  void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);

  unsigned int n;
  const unsigned int buf_samples_len;
  const unsigned int buf_size;

  bool playing;

  ALuint *src;
  ALuint *buf;
  std::vector<int16_t *> samples;
  ALCdevice *device;
  ALCcontext *context;
};

#endif  // NEWSPACEPLAYER_H
