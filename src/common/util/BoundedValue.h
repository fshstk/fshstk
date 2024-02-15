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

#pragma once
#include <spdlog/spdlog.h>

namespace fsh::util
{
/**
Represents a value that is bounded by a minimum and maximum.

Setting a value outside of the bounds will fail gracefully by clamping it to
the nearest bound and logging a warning.
*/
template<typename T, int MIN, int MAX>
class BoundedValue
{
public:
  /// Set the initial value
  BoundedValue(T val = {}) { set(val); }

  /// Get the value stored in this object
  auto get() const -> T { return _val; }

  /// Set a new value, clamping to min/max if necessary
  void set(T val)
  {
    if (val < min)
    {
      spdlog::warn("BoundedValue: value {} is below minimum {}, clamping", val, min);
      _val = min;
      return;
    }

    if (val > max)
    {
      spdlog::warn("BoundedValue: value {} is above maximum {}, clamping", val, max);
      _val = max;
      return;
    }

    _val = val;
  }

  /// Minimum value
  static constexpr auto min = static_cast<T>(MIN);

  /// Maximum value
  static constexpr auto max = static_cast<T>(MAX);

private:
  T _val;
};

template<int MIN, int MAX>
using BoundedFloat = BoundedValue<float, MIN, MAX>;

template<int MIN, int MAX>
using BoundedDouble = BoundedValue<double, MIN, MAX>;
} // namespace fsh::util
