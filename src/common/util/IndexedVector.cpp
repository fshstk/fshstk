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

#include "IndexedVector.h"
#include <spdlog/spdlog.h>

using namespace fsh::util;

void IndexedVector::resize(size_t newSize)
{
  data.resize(newSize);
  index = index % data.size();
}

auto IndexedVector::get() const -> float
{
  if (index >= data.size()) {
    spdlog::error("IndexedVector: index {} out of bounds (size: {}).", index, data.size());
    return 0.0f;
  }

  return data[index];
}
void IndexedVector::add(float val)
{
  if (index >= data.size()) {
    spdlog::error("IndexedVector: index {} out of bounds (size: {}).", index, data.size());
    return;
  }

  data[index] += val;
}

void IndexedVector::set(float val)
{
  if (index >= data.size()) {
    spdlog::error("IndexedVector: index {} out of bounds (size: {}).", index, data.size());
    return;
  }

  data[index] = val;
}

void IndexedVector::incrementIndex()
{
  index = (index + 1) % data.size();
}

void IndexedVector::clear()
{
  std::fill(data.begin(), data.end(), 0.0f);
}
