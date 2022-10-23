#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_opengl/juce_opengl.h>

void SHEval0(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval1(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval2(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval3(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval4(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval5(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval6(const float fX, const float fY, const float fZ, float* SHcoeffs);
void SHEval7(const float fX, const float fY, const float fZ, float* SHcoeffs);

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257389615890312154517
#endif

#ifndef sqrt4PI
#define sqrt4PI 3.544907701811032
#endif

// encoding and decoding with same direction and order yields the same encoded signal
// normalization is done at the decoding stage
constexpr float decodeCorrection(const int N)
{
  return sqrt4PI / (N + 1) / (N + 1);
}

inline void SHEval(const int N,
                   const float fX,
                   const float fY,
                   const float fZ,
                   float* SHcoeffs,
                   const bool doEncode = true)
{
  switch (N) {
    case 0:
      SHEval0(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(0), 1);
      break;
    case 1:
      SHEval1(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(1), 4);
      break;
    case 2:
      SHEval2(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(2), 9);
      break;
    case 3:
      SHEval3(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(3), 16);
      break;
    case 4:
      SHEval4(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(4), 25);
      break;
    case 5:
      SHEval5(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(5), 36);
      break;
    case 6:
      SHEval6(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(6), 49);
      break;
    case 7:
      SHEval7(fX, fY, fZ, SHcoeffs);
      juce::FloatVectorOperations::multiply(SHcoeffs, doEncode ? sqrt4PI : decodeCorrection(7), 64);
      break;
  }
}

inline void SHEval(const int N,
                   const juce::Vector3D<float> position,
                   float* pSH,
                   const bool doEncode = true)
{
  SHEval(N, position.x, position.y, position.z, pSH, doEncode);
}
