#ifndef PRESENTATIONPARAMS_H
#define PRESENTATIONPARAMS_H

class PresentationParams {
 public:
  PresentationParams()
      : presInterval(1),
        headAngle(0),
        waveType(WaveType::SINUS),
        pitchUse(PitchUse::NONE),
        timbreUse(TimbreUse::NONE) {}
  double presInterval;
  int headAngle;
  enum class WaveType { SINUS, TRIANGLE } waveType;
  enum class PitchUse { NONE, DISTANCE } pitchUse;
  enum class TimbreUse { NONE, WH_RATIO } timbreUse;
};

#endif  // PRESENTATIONPARAMS_H
