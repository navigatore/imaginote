#ifndef SPACEPLAYER_H
#define SPACEPLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdlib>
#include <vector>
#include "presentationparams.h"
#include "spaceobject.h"

class SpacePlayer {
 public:
  SpacePlayer(std::vector<SpaceObject> spaceObjects, PresentationParams params);
  ~SpacePlayer();

 private:
  static const unsigned int sample_rate = 44100;

  void addTone(PresentationParams::WaveType waveType, int16_t *buf,
               const unsigned int buf_len, float freq, float amp);
  void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);
  void addTriangularTone(int16_t *buf, const unsigned int buf_samples,
                         float freq, float amp);

  unsigned int n;
  std::vector<SpaceObject> spaceObjects;
  PresentationParams params;
  ALuint *src;
  ALuint *buf;
  std::vector<int16_t *> samples;
  ALCdevice *device;
};

#endif  // SPACEPLAYER_H
