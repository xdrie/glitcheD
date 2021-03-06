#include "Tuning.h"
#include "../defs.h"
#include <cmath>

// https://en.wikipedia.org/wiki/Equal_temperament#General_formulas_for_the_equal-tempered_interval
double glitched::note(uint16_t note) { return 440 * std::pow(SEMITONE_RATIO, note - 49); }

double glitched::detune(double freq, int cents) { return freq * std::pow(CENT_RATIO, cents); }
