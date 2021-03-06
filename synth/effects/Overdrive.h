//
// Created by xdrie on 10/9/19.
//
#pragma once

#include "../mod/Parameter.h"
#include "Effect.h"

namespace glitched {
class Overdrive : public Effect {
  public:
    Knob gain;
    Knob threshold;

    explicit Overdrive(Knob gain, Knob threshold);

    virtual double value(double v, double t) override;
};
} // namespace glitched
