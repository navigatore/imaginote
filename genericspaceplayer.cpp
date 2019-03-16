#include "genericspaceplayer.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <cstring>

const float pi = static_cast<float>(M_PI);

GenericSpacePlayer::GenericSpacePlayer()
    : playing(false),
      buf_samples_len(sample_rate),
      buf_size(buf_samples_len * 2),
      curTime(0) {
  // OpenAL initialization
  device = alcOpenDevice(nullptr);
  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  n = 1;

  // sources and buffer initialization

  src = new ALuint[n];
  alGenSources(static_cast<int>(n), src);

  buf = new ALuint[n];
  alGenBuffers(static_cast<int>(n), buf);

  const unsigned int buf_samples_len = static_cast<unsigned int>(sample_rate);
  const unsigned int buf_size = buf_samples_len * 2;  // 16-bit == 2 bytes

  samples.resize(n);

  samples[0] = new int16_t[buf_size];

  std::memset(samples[0], 0, buf_size);
}

void GenericSpacePlayer::updateListenerPosition(const Coordinates &pos,
                                                const Angle &angle) {
  listenerPos = pos.to2d();
  alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
  float listener_pos[] = {
      std::cos(angle.getRad()), 0, -std::sin(angle.getRad()), 0, 1, 0};
  alListenerfv(AL_ORIENTATION, listener_pos);
}

void GenericSpacePlayer::update(float duration) { curTime += duration; }

void GenericSpacePlayer::updateSonifiedPointPosition(const Coordinates2d &pos) {
  sonifiedPointPos = pos;
  alSource3f(src[0], AL_POSITION, pos.x, 0, pos.y);
}

void GenericSpacePlayer::startPlaying() {
  if (!playing) {
    alSourcei(src[0], AL_BUFFER, static_cast<ALint>(buf[0]));
    alSourcei(src[0], AL_LOOPING, 1);
    alSourcePlay(src[0]);
    playing = true;
  }
}

void GenericSpacePlayer::setSonificationObject(const SimpleSpaceObject &obj) {
  std::memset(samples[0], 0, buf_size);
  addSinusoidalTone(samples[0], buf_samples_len, 440.0f / obj.height, 0.8f);
  stopPlaying();
  alBufferData(buf[0], AL_FORMAT_MONO16, samples[0],
               static_cast<ALsizei>(buf_size), sample_rate);
  startPlaying();
}

void GenericSpacePlayer::stopPlaying() {
  if (playing) {
    alSourceStop(src[0]);
    alSourcei(src[0], AL_BUFFER, 0);
    playing = false;
  }
}

void GenericSpacePlayer::addSinusoidalTone(int16_t *buf,
                                           const unsigned int buf_samples,
                                           float freq, float amp) {
  int umax = (1 << 15) - 1;

  for (unsigned int i = 0; i < buf_samples; ++i) {
    buf[i] += static_cast<int16_t>(
        umax * amp * std::sin((2.0f * pi * freq) / sample_rate * i));
  }
}

GenericSpacePlayer::~GenericSpacePlayer() {
  for (unsigned int i = 0; i < n; ++i) {
    alSourceStop(src[i]);
  }

  for (auto ptr : samples) {
    delete[] ptr;
  }

  alDeleteSources(static_cast<int>(n), src);
  alDeleteBuffers(static_cast<int>(n), buf);
  delete[] src;
  delete[] buf;

  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);

  alcCloseDevice(device);
}
