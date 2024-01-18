/***************************************************************************************************
                 ██████          █████                              █████    █████
                ███░░███        ░░███                              ░░███    ░░███
               ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████
              ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███
             ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░
               ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███
               █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████
             ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░

            fantastic  spatial  holophonic                software    tool    kit

                                    copyright (c) fabian hummel
                                       www.github.com/fshstk
                                           www.fshstk.com

         this file is part of the fantastic spatial holophonic software toolkit (fsh::stk)
  fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0
                                    www.gnu.org/licenses/gpl-3.0
***************************************************************************************************/

#pragma once
#include <spdlog/spdlog.h>

namespace fsh {
template<typename T, int MIN, int MAX>
class BoundedValue
{
public:
  BoundedValue(T val) { set(val); }

  auto get() const -> T { return _val; }

  void set(T val)
  {
    if (_val < min) {
      spdlog::warn("BoundedValue: value {} is below minimum {}, clamping", _val, min);
      _val = MIN;
      return;
    }

    if (_val > max) {
      spdlog::warn("BoundedValue: value {} is above maximum {}, clamping", _val, min);
      _val = max;
      return;
    }

    _val = val;
  }

  static constexpr auto min = static_cast<T>(MIN);
  static constexpr auto max = static_cast<T>(MAX);

private:
  T _val;
};

template<int MIN, int MAX>
using BoundedFloat = BoundedValue<float, MIN, MAX>;
} // namespace fsh
