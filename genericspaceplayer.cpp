#include "genericspaceplayer.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <cstring>
#include "nvgmath.h"

GenericSpacePlayer::GenericSpacePlayer()
    : playing(false),
      primaryIdx(0),
      volumeMultiplier(1.0f),
      primaryGain(0.0f),
      secondaryGain(0.0f),
      primaryFrequency(0.0f) {
  device = alcOpenDevice(nullptr);
  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  src = new ALuint[2];
  alGenSources(static_cast<int>(2), src);

  buf = new ALuint[2];
  alGenBuffers(static_cast<int>(2), buf);

  samples.resize(2);
  samples[0] = new int16_t[buf_size];
  samples[1] = new int16_t[buf_size];

  std::memset(samples[0], 0, buf_size);
  std::memset(samples[1], 0, buf_size);

  auto secondaryIdx = (primaryIdx + 1) % 2;

  alBufferData(buf[primaryIdx], AL_FORMAT_MONO16, samples[primaryIdx],
               static_cast<ALsizei>(buf_size), sample_rate);
  alBufferData(buf[secondaryIdx], AL_FORMAT_MONO16, samples[primaryIdx],
               static_cast<ALsizei>(buf_size), sample_rate);

  for (unsigned int i = 0; i < 2; ++i) {
    startSource(i);
  }
}

void GenericSpacePlayer::updateListenerPosition(const Coordinates &pos,
                                                const Angle &angle) {
  listenerPos = pos;
  alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
  float listener_pos[] = {
      std::cos(angle.getRad()), 0, -std::sin(angle.getRad()), 0, 1, 0};
  alListenerfv(AL_ORIENTATION, listener_pos);
}

void GenericSpacePlayer::update(float duration) { updateGains(duration); }

void GenericSpacePlayer::updateGains(float duration) {
  auto gainStep = duration / fadeTime;
  if (!playing) {
    primaryGain = std::max(primaryGain - gainStep, 0.0f);
    secondaryGain = std::max(secondaryGain - gainStep, 0.0f);
    setGains();
  } else if (primaryGain != 1.0f) {
    primaryGain = std::min(primaryGain + gainStep, 1.0f);
    secondaryGain = std::max(secondaryGain - gainStep, 0.0f);
    setGains();
  }
}

void GenericSpacePlayer::volumeUp() {
  volumeMultiplier += volumeMultiplierChangeStep;
  alListenerf(AL_GAIN, volumeMultiplier);
}

void GenericSpacePlayer::volumeDown() {
  volumeMultiplier -= volumeMultiplierChangeStep;
  if (volumeMultiplier < 0) {
    volumeMultiplier = 0;
  }
  alListenerf(AL_GAIN, volumeMultiplier);
}

float GenericSpacePlayer::getVolume() const { return volumeMultiplier; }

float GenericSpacePlayer::getFrequency(unsigned int height) {
  return baseFrequency / height;
}

void GenericSpacePlayer::stopPrimary() { stopSource(primaryIdx); }

void GenericSpacePlayer::startPrimary() { startSource(primaryIdx); }

void GenericSpacePlayer::updateSonifiedPointPosition(const Coordinates2d &pos) {
  sonifiedPointPos = pos;
  alSource3f(src[primaryIdx], AL_POSITION, pos.x, 0, pos.y);
}

void GenericSpacePlayer::startSource(unsigned int idx) {
  alSourcei(src[idx], AL_BUFFER, static_cast<ALint>(buf[idx]));
  alSourcei(src[idx], AL_LOOPING, 1);
  alSourcePlay(src[idx]);
}

void GenericSpacePlayer::startPlaying() { playing = true; }

void GenericSpacePlayer::setSonificationObject(const SimpleSpaceObject &obj) {
  auto frequency = getFrequency(obj.height);
  if (nvg::almostEqual(frequency, primaryFrequency)) {
    return;
  }
  primaryFrequency = frequency;
  primaryIdx = (primaryIdx + 1) % 2;
  secondaryGain = primaryGain;
  primaryGain = 0.0f;
  std::memset(samples[primaryIdx], 0, buf_size);
  auto period = std::min(static_cast<unsigned int>(sample_rate / frequency),
                         static_cast<unsigned int>(buf_size));
  auto lengthInBytes = period * 2;

  addSinusoidalTone(samples[primaryIdx], period, frequency, 0.8f);
  stopPrimary();
  alBufferData(buf[primaryIdx], AL_FORMAT_MONO16, samples[primaryIdx],
               static_cast<ALsizei>(lengthInBytes), sample_rate);
  startPrimary();
}

void GenericSpacePlayer::stopSource(unsigned int idx) {
  alSourceStop(src[idx]);
  alSourcei(src[idx], AL_BUFFER, 0);
}

void GenericSpacePlayer::stopPlaying() { playing = false; }

void GenericSpacePlayer::addSinusoidalTone(int16_t *buf,
                                           const unsigned int buf_samples,
                                           float freq, float amp) {
  int umax = (1 << 15) - 1;

  for (unsigned int i = 0; i < buf_samples; ++i) {
    buf[i] += static_cast<int16_t>(
        umax * amp * std::sin((2.0f * nvg::pi * freq) / sample_rate * i));
  }
}

void GenericSpacePlayer::setGains() {
  auto secondaryIdx = (primaryIdx + 1) % 2;
  alSourcef(src[primaryIdx], AL_GAIN, primaryGain);
  alSourcef(src[secondaryIdx], AL_GAIN, secondaryGain);
}

GenericSpacePlayer::~GenericSpacePlayer() {
  alSourceStop(src[0]);
  alSourceStop(src[1]);

  for (auto ptr : samples) {
    delete[] ptr;
  }

  alDeleteSources(static_cast<int>(2), src);
  alDeleteBuffers(static_cast<int>(2), buf);
  delete[] src;
  delete[] buf;

  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);

  alcCloseDevice(device);
}
