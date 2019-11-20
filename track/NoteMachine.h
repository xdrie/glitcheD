#pragma once

#include "../synth/SaltSynth.h"
#include "../util/StereoSample.h"
#include <cstdint>
#include <vector>

namespace glitched {
class NoteMachine {
    using byte = uint8_t;

    std::vector<byte> memory;
    int programCounter;
    int stackPointer;

    byte noteDuration = 24;
    byte noteBps = 48;
    byte noteVelocity = 128;

    byte noteMode = MODE_IMMEDIATE;

    uint32_t bufferPosition;

    const byte MODE_IMMEDIATE = 0x00;
    const byte MODE_COMPOSE = 0x01;

    const byte FRAME_DELIMITER = 0x00;

    double getDuration();
    double getVelocity();
    void copyAudio(StereoSample buffer, uint32_t position);

  public:
    Instrument &instrument;
    StereoSample audioBuffer;

    enum Instruction : byte {
        NOP = 0x00,
        MODE = 0x01,
        PLAY = 0x10,
        PUSH = 0x11,
        FRAME = 0x12,
        BATCH = 0x13,
        REST = 0x14,
        BACK = 0x15,
        DUR = 0x20,
        VEL = 0x21,
        BPS = 0x22,
        RET = 0x99,
    };

    explicit NoteMachine(uint16_t memorySize, Instrument &instrument);
    void loadProgram(std::vector<byte> program);
    void execute();
};
} // namespace glitched