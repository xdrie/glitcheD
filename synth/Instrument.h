//
// Created by xdrie on 10/29/19.
//
#pragma once

#include "../util/StereoSample.h"
#include <cstdint>
#include <vector>

namespace glitched {
class Instrument {
  public:
    virtual glitched::StereoSample play(uint16_t note, double dur, double vol) = 0;
};
} // namespace glitched
