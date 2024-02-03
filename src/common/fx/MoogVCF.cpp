/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic               synthesis    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#include "MoogVCF.h"
#include "spdlog/spdlog.h"

using namespace fsh::fx;
using Base = juce::dsp::LadderFilter<float>;

void MoogVCF::setParams(const Params& params)
{
  Base::setCutoffFrequencyHz(params.cutoff.get());
  Base::setResonance(params.resonance.get());
  Base::setDrive(params.drive.get());
}

void MoogVCF::setSampleRate(double sampleRate)
{
  Base::prepare({ .sampleRate = sampleRate, .maximumBlockSize = 1, .numChannels = 1 });
}

float MoogVCF::processSample(float input)
{
  return Base::processSample(input, 0);
}

void MoogVCF::reset()
{
  Base::reset();
}
