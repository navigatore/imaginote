#include "genericspaceplayer.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <cmath>
#include <cstring>
#include <limits>

#include "nvgmath.h"

GenericSpacePlayer::GenericSpacePlayer() {
  device = alcOpenDevice(nullptr);
  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  alGenSources(static_cast<int>(noOfBuffers), src.data());
  alGenBuffers(static_cast<int>(noOfBuffers), buf.data());

  auto secondaryIdx = (primaryIdx + 1) % 2;

  alBufferData(buf.at(primaryIdx), AL_FORMAT_MONO16, samples.data(),
               static_cast<ALsizei>(buf_size), sample_rate);
  alBufferData(buf.at(secondaryIdx), AL_FORMAT_MONO16, samples.data(),
               static_cast<ALsizei>(buf_size), sample_rate);

  samples.fill(0);
  for (const auto &freq : warnFreqs) {
    addSinusoidalTone(freq, warnAmplitude);
    fadeIn();
    fadeOut();
  }

  alBufferData(buf[visitedWarnIdx], AL_FORMAT_MONO16, samples.data(),
               static_cast<ALsizei>(buf_size), sample_rate);

  for (unsigned int i = 0; i < noOfBuffers; ++i) {
    startSource(i);
    alSourcef(i, AL_REFERENCE_DISTANCE, referenceDistance);
  }
}

void GenericSpacePlayer::updateListenerPosition(const Coordinates &pos,
                                                const Angle &angle) {
  _listenerPos = pos;
  alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z());
  std::array listener_pos{std::cos(angle.getRad()),
                          0.0F,
                          -std::sin(angle.getRad()),
                          0.0F,
                          1.0F,
                          0.0F};
  alListenerfv(AL_ORIENTATION, listener_pos.data());
  alSource3f(src.at(visitedWarnIdx), AL_POSITION, _listenerPos.x(), 0.0F,
             _listenerPos.y());
}

void GenericSpacePlayer::updateTime(float duration) { updateGains(duration); }

void GenericSpacePlayer::setVolume(float volume) {
  alListenerf(AL_GAIN, volume);
}

void GenericSpacePlayer::updateGains(float duration) {
  auto gainStep = duration / fadeTime;
  if (!playing) {
    primaryGain = std::max(primaryGain - gainStep, 0.0F);
    secondaryGain = std::max(secondaryGain - gainStep, 0.0F);
  } else if (primaryGain != 1.0F) {
    primaryGain = std::min(primaryGain + gainStep, 1.0F);
    secondaryGain = std::max(secondaryGain - gainStep, 0.0F);
  }
  if (standingField != nullptr) {
    if (standingField->visited()) {
      warnGain = std::min(warnGain + gainStep, 1.0F);
    } else {
      warnGain = std::max(warnGain - gainStep, 0.0F);
    }
  }
  setGains();
}

constexpr float GenericSpacePlayer::getFrequency(unsigned int height) {
  return baseFrequency / static_cast<float>(height);
}

const Coordinates2d &GenericSpacePlayer::sonifiedPointPos() const {
  return _sonifiedPointPos;
}

const Coordinates2d &GenericSpacePlayer::listenerPos() const {
  return _listenerPos;
}

void GenericSpacePlayer::stopPrimary() { stopSource(primaryIdx); }

void GenericSpacePlayer::startPrimary() { startSource(primaryIdx); }

void GenericSpacePlayer::updateSonifiedPointPosition(const Coordinates2d &pos) {
  _sonifiedPointPos = pos;
  alSource3f(src.at(primaryIdx), AL_POSITION, pos.x(), 0, pos.y());
}

void GenericSpacePlayer::startSource(unsigned int idx) {
  alSourcei(src.at(idx), AL_BUFFER, static_cast<ALint>(buf.at(idx)));
  alSourcei(src.at(idx), AL_LOOPING, 1);
  alSourcePlay(src.at(idx));
}

void GenericSpacePlayer::startPlaying() { playing = true; }

void GenericSpacePlayer::setSonificationObject(const SimpleSpaceObject &obj) {
  auto frequency = getFrequency(obj.height());
  if (nvg::almostEqual(frequency, primaryFrequency)) {
    return;
  }
  primaryFrequency = frequency;
  primaryIdx = (primaryIdx + 1) % 2;
  secondaryGain = primaryGain;
  primaryGain = 0.0F;
  samples.fill(0);
  auto period = std::min(static_cast<unsigned int>(sample_rate / frequency),
                         static_cast<unsigned int>(samplesLen));
  auto lengthInBytes = period * 2;

  addSinusoidalTone(frequency, defaultAmplitude, period);
  stopPrimary();
  alBufferData(buf.at(primaryIdx), AL_FORMAT_MONO16, samples.data(),
               static_cast<ALsizei>(lengthInBytes), sample_rate);
  startPrimary();
}

void GenericSpacePlayer::setStandingField(const SimpleSpaceObject &obj) {
  standingField = &obj;
}

void GenericSpacePlayer::stopSource(unsigned int idx) {
  alSourceStop(src.at(idx));
  alSourcei(src.at(idx), AL_BUFFER, 0);
}

void GenericSpacePlayer::stopPlaying() { playing = false; }

void GenericSpacePlayer::addSinusoidalTone(float freq, float amp,
                                           unsigned int samplesLength) {
  auto umax = static_cast<float>(std::numeric_limits<int16_t>::max());

  for (unsigned int i = 0; i < samplesLength; ++i) {
    samples.at(i) += static_cast<int16_t>(
        umax * amp *
        std::sin((nvg::fullAngleDeg * freq * static_cast<float>(i)) /
                 static_cast<float>(sample_rate)));
  }
}

void GenericSpacePlayer::setGains() {
  auto secondaryIdx = (primaryIdx + 1) % 2;
  alSourcef(src.at(primaryIdx), AL_GAIN, primaryGain);
  alSourcef(src.at(secondaryIdx), AL_GAIN, secondaryGain);
  alSourcef(src[visitedWarnIdx], AL_GAIN, warnGain);
}

GenericSpacePlayer::~GenericSpacePlayer() {
  for (const auto &source : src) {
    alSourceStop(source);
  }

  alDeleteSources(static_cast<int>(noOfBuffers), src.data());
  alDeleteBuffers(static_cast<int>(noOfBuffers), buf.data());

  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);

  alcCloseDevice(device);
}

void GenericSpacePlayer::fadeIn() {
  for (unsigned int i = 0; i < fadeSamples; ++i) {
    auto sampleF = static_cast<float>(samples.at(i));
    auto iF = static_cast<float>(i);
    auto fadeSamplesF = static_cast<float>(fadeSamples);
    samples.at(i) = static_cast<int16_t>(sampleF * iF / fadeSamplesF);
  }
}

void GenericSpacePlayer::fadeOut() {
  for (unsigned int i = samplesLen - fadeSamples; i < samplesLen; ++i) {
    auto sampleF = static_cast<float>(samples.at(i));
    auto samplesLenF = static_cast<float>(samplesLen);
    auto iF = static_cast<float>(i);
    auto fadeSamplesF = static_cast<float>(fadeSamples);
    samples.at(i) =
        static_cast<int16_t>(sampleF * (samplesLenF - iF) / fadeSamplesF);
  }
}
