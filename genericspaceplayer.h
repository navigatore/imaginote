#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <array>
#include <cstdlib>
#include <vector>
#include "angle.h"
#include "simplespaceobject.h"

class GenericSpacePlayer {
 public:
  GenericSpacePlayer();
  GenericSpacePlayer(const GenericSpacePlayer &) = delete;
  GenericSpacePlayer(GenericSpacePlayer &&) = delete;
  GenericSpacePlayer &operator=(const GenericSpacePlayer &) = delete;
  GenericSpacePlayer &operator=(GenericSpacePlayer &&) = delete;
  virtual ~GenericSpacePlayer();

  virtual void startPlaying();
  virtual void stopPlaying();
  virtual void setSonificationObject(const SimpleSpaceObject &obj);
  virtual void setStandingField(const SimpleSpaceObject &obj);
  virtual void updateSonifiedPointPosition(const Coordinates2d &pos);
  virtual void updateListenerPosition(const Coordinates &pos,
                                      const Angle &angle);
  virtual void updateTime(float duration);
  virtual void setVolume(float volume);

  [[nodiscard]] virtual std::string getName() const noexcept = 0;

 protected:
  virtual void updateGains(float duration);

  [[nodiscard]] const Coordinates2d &listenerPos() const;
  [[nodiscard]] const Coordinates2d &sonifiedPointPos() const;

 private:
  static constexpr unsigned int sample_rate = 44100;
  static constexpr unsigned int samplesLen = sample_rate;
  static constexpr unsigned int buf_size = 2 * samplesLen;
  static constexpr float referenceDistance = 0.3F;
  static constexpr float fadeTime = 0.1F;
  static constexpr unsigned int fadeSamples =
      static_cast<unsigned int>(sample_rate * fadeTime);
  static constexpr float baseFrequency = 440.0F;
  static constexpr float defaultAmplitude = 0.1F;
  static constexpr float warnAmplitude = 0.3F * defaultAmplitude;
  static constexpr unsigned int noOfBuffers = 3;
  static constexpr unsigned int visitedWarnIdx = 2;
  static constexpr std::array warnFreqs{311.13F, 349.23F};

  void startPrimary();
  void stopPrimary();
  void startSource(unsigned int idx);
  void stopSource(unsigned int idx);

  void addSinusoidalTone(float freq, float amp,
                         unsigned int samplesLength = samplesLen);
  void fadeIn();
  void fadeOut();

  void setGains();

  float getFrequency(unsigned int height);

  Coordinates2d _listenerPos, _sonifiedPointPos;
  bool playing{false};
  unsigned int primaryIdx{};

  float primaryGain{}, secondaryGain{}, warnGain{};
  float primaryFrequency{};
  std::array<ALuint, noOfBuffers> src{};
  std::array<ALuint, noOfBuffers> buf{};
  std::array<int16_t, samplesLen> samples{};
  ALCdevice *device;
  ALCcontext *context;
  const SimpleSpaceObject *standingField{};
};
