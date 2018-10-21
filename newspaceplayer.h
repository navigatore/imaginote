#ifndef NEWSPACEPLAYER_H
#define NEWSPACEPLAYER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdlib>
#include <vector>

class NewSpacePlayer
{
public:
    NewSpacePlayer();
    ~NewSpacePlayer();

private:
    static const unsigned int sample_rate = 44100;

    void addSinusoidalTone(int16_t *buf, const unsigned int buf_samples, float freq, float amp);

    unsigned int n;
    ALuint *src;
    ALuint *buf;
    std::vector<int16_t*> samples;
    ALCdevice *device;
    ALCcontext *context;
};

#endif // NEWSPACEPLAYER_H
