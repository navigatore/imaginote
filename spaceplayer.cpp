#include "spaceplayer.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <cstring>

const float pi = static_cast<float>(M_PI);

void SpacePlayer::addTone(PresentationParams::WaveType waveType, int16_t *buf,
                          const unsigned int buf_samples, float freq,
                          float amp) {
  if (waveType == PresentationParams::WaveType::SINUS) {
    addSinusoidalTone(buf, buf_samples, freq, amp);
  } else if (waveType == PresentationParams::WaveType::TRIANGLE) {
    addTriangularTone(buf, buf_samples, freq, amp);
  }

  unsigned int faded_samples =
      std::min(sample_rate / 20, buf_samples);  // for 50-ms fade-in / fade-out

  for (unsigned int i = 0; i < faded_samples; ++i) {
    double coeff = static_cast<double>(i) / faded_samples;
    buf[i] *= coeff;
    buf[buf_samples - i - 1] *= coeff;
  }
}

void SpacePlayer::addSinusoidalTone(int16_t *buf,
                                    const unsigned int buf_samples, float freq,
                                    float amp) {
  int umax = (1 << 15) - 1;

  for (unsigned int i = 0; i < buf_samples; ++i) {
    buf[i] += static_cast<int16_t>(
        umax * amp * std::sin((2.0f * pi * freq) / sample_rate * i));
  }
}

void SpacePlayer::addTriangularTone(int16_t *buf,
                                    const unsigned int buf_samples, float freq,
                                    float amp) {
  int umax = (1 << 15) - 1;

  unsigned int quarter_samples = sample_rate / freq / 4;
  float step = amp / quarter_samples;
  unsigned int quarter = 0;
  float cur = 0.0f;

  for (unsigned int i = 0; i < buf_samples; ++i) {
    if (i % quarter_samples == 0) quarter = (quarter + 1) % 4;
    buf[i] = static_cast<int16_t>(umax * cur);
    if (quarter == 0 || quarter == 3)
      cur += step;
    else
      cur -= step;
  }
}

SpacePlayer::SpacePlayer(std::vector<SpaceObject> spaceObjects,
                         PresentationParams params)
    : spaceObjects(spaceObjects), params(params), src(nullptr), buf(nullptr) {
  // OpenAL initialization
  device = alcOpenDevice(nullptr);
  auto context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  n = static_cast<unsigned int>((spaceObjects.size()));

  // src, buf initialization

  src = new ALuint[n];
  buf = new ALuint[n];

  alGenBuffers(static_cast<int>(n), buf);
  alGenSources(static_cast<int>(n), src);

  const unsigned int buf_nonzero_samples =
      static_cast<unsigned int>(sample_rate * params.presInterval);
  const unsigned int buf_samples = buf_nonzero_samples * n;
  const unsigned int buf_size = buf_samples * 2;  // 16-bit == 2 bytes

  samples.resize(n);
  for (unsigned int i = 0; i < n; ++i) {
    samples[i] = new int16_t[buf_size];
    memset(samples[i], 0, buf_size);
  }

  for (unsigned int i = 0; i < spaceObjects.size(); ++i) {
    if (params.pitchUse == PresentationParams::PitchUse::DISTANCE &&
        params.timbreUse == PresentationParams::TimbreUse::NONE) {
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples,
              880.0f / spaceObjects[i].getDistanceFromCenter(), 0.5);
    } else if (params.pitchUse == PresentationParams::PitchUse::NONE &&
               params.timbreUse == PresentationParams::TimbreUse::WH_RATIO) {
      auto ratio = spaceObjects[i].getWHRatio();
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples, 880.0, 0.5f * ratio);
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples, 1320.0, 0.5f * (1 - ratio));
    } else if (params.pitchUse == PresentationParams::PitchUse::DISTANCE &&
               params.timbreUse == PresentationParams::TimbreUse::WH_RATIO) {
      auto ratio = spaceObjects[i].getWHRatio();
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples,
              4 * 440.0f / spaceObjects[i].getDistanceFromCenter(),
              0.5f * ratio);
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples,
              6 * 440.0f / spaceObjects[i].getDistanceFromCenter(),
              0.5f * (1 - ratio));
    } else {
      addTone(params.waveType, samples[i] + i * buf_nonzero_samples,
              buf_nonzero_samples, 440.0, 0.5);
    }
  }

  for (unsigned int i = 0; i < n; ++i) {
    alBufferData(buf[i], AL_FORMAT_MONO16, samples[i],
                 static_cast<ALsizei>(buf_size), sample_rate);
    alSourcei(src[i], AL_BUFFER, static_cast<ALint>(buf[i]));
    alSourcei(src[i], AL_LOOPING, 1);
  }

  for (unsigned int i = 0; i < n; ++i) {
    auto center = spaceObjects[i].getCenter();
    alSource3f(src[i], AL_POSITION, center.x, center.y, center.z);
  }

  auto headAngleRad = params.headAngle / 180.0 * M_PI;
  ALfloat listener_pos[] = {static_cast<ALfloat>(std::sin(headAngleRad)),
                            0,
                            static_cast<ALfloat>(-std::cos(headAngleRad)),
                            0,
                            1,
                            0};
  alListenerfv(AL_ORIENTATION, listener_pos);

  for (unsigned int i = 0; i < n; ++i) {
    alSourcePlay(src[i]);
  }
}

SpacePlayer::~SpacePlayer() {
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

  alcCloseDevice(device);
}
