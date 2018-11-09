#include "newspaceplayer.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <cstring>

const float pi = static_cast<float>(M_PI);

//*********************************************************************************************************************
NewSpacePlayer::NewSpacePlayer() : buf_samples_len(sample_rate), buf_size(buf_samples_len * 2), playing(false)
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

    const unsigned int buf_samples_len = static_cast<unsigned int>(sample_rate);
    const unsigned int buf_size = buf_samples_len * 2;  // 16-bit == 2 bytes

    samples.resize(n);

    samples[0] = new int16_t[buf_size];

    std::memset(samples[0], 0, buf_size);
}
//*********************************************************************************************************************
void NewSpacePlayer::updateListenerPosition(Coordinates pos)
{
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}
//*********************************************************************************************************************
void NewSpacePlayer::sonificateObject(SimpleSpaceObject obj)
{
    if (playing)
    {
        alSourceStop(src[0]);
        alSourcei(src[0], AL_BUFFER, 0);
    }

    std::memset(samples[0], 0, buf_size);

    addSinusoidalTone(samples[0], buf_samples_len, 440.0f / obj.height, 0.8f);
    alBufferData(buf[0], AL_FORMAT_MONO16, samples[0], static_cast<ALsizei>(buf_size), sample_rate);

    alSourcei(src[0], AL_BUFFER, static_cast<ALint>(buf[0]));
    alSourcei(src[0], AL_LOOPING, 1);

    alSource3f(src[0], AL_POSITION, obj.crds.x, obj.crds.y, obj.crds.z);

    alSourcePlay(src[0]);

    playing = true;
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
