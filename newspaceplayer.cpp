#include "newspaceplayer.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <cstring>

const float pi = static_cast<float>(M_PI);

//*********************************************************************************************************************
NewSpacePlayer::NewSpacePlayer()
{
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

    const unsigned int buf_samples = static_cast<unsigned int>(sample_rate * n);
    const unsigned int buf_size = buf_samples * 2;  // 16-bit == 2 bytes

    samples.resize(n);

    samples[0] = new int16_t[buf_size];
    std::memset(samples[0], 0, buf_size);

    addSinusoidalTone(samples[0], buf_samples, 440.0f, 0.8f);
    alBufferData(buf[0], AL_FORMAT_MONO16, samples[0], static_cast<ALsizei>(buf_size), sample_rate);

    alSourcei(src[0], AL_BUFFER, static_cast<ALint>(buf[0]));
    alSourcei(src[0], AL_LOOPING, 1);

    alSource3f(src[0], AL_POSITION, 0, 0, 0);

    alSourcePlay(src[0]);
}
//*********************************************************************************************************************
void NewSpacePlayer::addSinusoidalTone(int16_t *buf, const unsigned int buf_samples, float freq, float amp)
{
    int umax = (1 << 15) - 1;

    for (unsigned int i = 0; i < buf_samples; ++i)
    {
        buf[i] += static_cast<int16_t>(umax * amp * std::sin( (2.0f * pi * freq) / sample_rate * i ));
    }
}
//*********************************************************************************************************************
NewSpacePlayer::~NewSpacePlayer()
{
    for (unsigned int i = 0; i < n; ++i)
    {
        alSourceStop(src[i]);
    }

    for (auto ptr : samples)
    {
        delete [] ptr;
    }

    alDeleteSources(static_cast<int>(n), src);
    alDeleteBuffers(static_cast<int>(n), buf);
    delete [] src;
    delete [] buf;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);

    alcCloseDevice(device);
}
//*********************************************************************************************************************
