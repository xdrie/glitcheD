#include "NoteMachine.h"
#include "../defs.h"

glitched::NoteMachine::NoteMachine(uint16_t memorySize, Instrument &instrument)
    : programCounter(0), bufferPosition(0), instrument(instrument) {
    this->memory.resize(memorySize);
    stackPointer = memorySize - 1;
}

void glitched::NoteMachine::loadProgram(std::vector<uint8_t> program) {
    for (size_t i = 0; i < program.size(); i++) {
        this->memory[i] = program[i];
    }
}

void glitched::NoteMachine::execute() {
    bool running = true;
    while (running) {
        auto instr = static_cast<Instruction>(this->memory[programCounter]);

        switch (instr) {
        case NOP: {
            break;
        }
        case MODE: {
            byte mode = this->memory[programCounter++ + 1];
            noteMode = mode;
            break;
        }
        case PLAY: {
            byte note = this->memory[programCounter++ + 1];
            auto dur = getDuration();
            auto noteSample = instrument.play(note, dur, getVelocity());
            copyAudio(noteSample, bufferPosition);
            bufferPosition += dur * SAMPLE_RATE;
            break;
        }
        case REST: {
            auto dur = getDuration();
            bufferPosition += dur * SAMPLE_RATE;
            break;
        }
        case BACK: {
            auto dur = getDuration();
            bufferPosition -= dur * SAMPLE_RATE;
            break;
        }
        case PUSH: {
            byte note = this->memory[programCounter++ + 1];
            this->memory[stackPointer--] = note;
            break;
        }
        case FRAME: {
            this->memory[stackPointer--] = FRAME_DELIMITER;
            break;
        }
        case BATCH: {
            auto dur = getDuration();
            while (this->memory[++stackPointer] != FRAME_DELIMITER) {
                byte note = this->memory[stackPointer];
                auto noteBuffer = instrument.play(note, dur, getVelocity());
                copyAudio(noteBuffer, bufferPosition);
            }
            bufferPosition += dur * SAMPLE_RATE;
            break;
        }
        case DUR: {
            noteDuration = this->memory[programCounter++ + 1];
            break;
        }
        case VEL: {
            noteVelocity = this->memory[programCounter++ + 1];
            break;
        }
        case BPS: {
            noteBps = this->memory[programCounter++ + 1];
            break;
        }
        case RET: {
            running = false;
        }
        }
        programCounter++;
    }
}

double glitched::NoteMachine::getDuration() {
    return noteDuration / static_cast<double>(noteBps);
}

double glitched::NoteMachine::getVelocity() { return noteVelocity / 128.0; }

void glitched::NoteMachine::copyAudio(StereoSample buffer, uint32_t position) {
    // auto-resize audio buffer
    auto targetSize = position + buffer.l.size();
    if (audioBuffer.l.size() < targetSize) {
        audioBuffer.resize(targetSize);
    }
    StereoSample::copy(buffer, audioBuffer, position);
}
