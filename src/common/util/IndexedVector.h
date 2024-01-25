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
#include <vector>

namespace fsh::util {
/**
Primitive ring buffer class.

Can be used as a simple delay line or FIFO queue.
*/
class IndexedVector
{
public:
  /// Resize the underlying vector.
  void resize(size_t newSize);

  /// Get the element at the current index.
  auto get() const -> float;

  /// Add a value to the element at the current index.
  void add(float val);

  /// Set the element at the current index.
  void set(float val);

  /// Increment the index with wraparound.
  void incrementIndex();

  /// Set all elements to zero.
  void clear();

private:
  size_t index = 0;
  std::vector<float> data;
};
} // namespace fsh::util
