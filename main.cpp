
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "constants.h"
#include "track/SongWriter.h"
#include "synth/Oscillator.h"
#include "synth/Filter.h"
#include "util/Tuning.h"
#include "synth/Instrument.h"
#include "track/NoteMachine.h"
#include "misc/songs.h"
#include "synth/mod/LFO.h"
#include "synth/effects/Overdrive.h"

int main(int argc, const char *argv[]) {
    auto osc1 = glitched::Oscillator(glitched::Wave::Saw);
    auto osc2 = glitched::Oscillator(glitched::Wave::Square);

    auto test440Buf = osc1.play(glitched::note(49), 1, 0.9f);

    auto voices = {osc1, osc2};
    auto ampEnv = glitched::Envelope(0.05f, 0.2f, 0.7f, 0.4f);
    auto cutoffMod = glitched::LFO(2.0);
    auto cutoff = glitched::Value(0.6, cutoffMod);
    cutoff.modAmount = 0.2;
    auto resonance = glitched::Value(0.01);
    auto filter = glitched::Filter(glitched::FilterMode::LowPass, cutoff, resonance);
    auto instr1 = glitched::Instrument(voices, ampEnv, filter);

    instr1.effects.emplace_back(std::make_unique<glitched::Overdrive>(glitched::Value(2.0), glitched::Value(0.8)));

    glitched::NoteMachine noteMachine(INT16_MAX, instr1, 32);
    noteMachine.loadProgram(demo_stardustCrusaders);
    noteMachine.execute();

    glitched::SongWriter songWriter(noteMachine.audioBuffer.size());
    songWriter.insert(0, noteMachine.audioBuffer);
    songWriter.save("out.wav");
}

