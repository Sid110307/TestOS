#pragma once

#include "renderer.h"

struct InterruptFrame
{
    size_t instructionPointer, codeSegment;
    size_t cpuFlags, stackPointer, stackSegment;
};

namespace Interrupts
{
    __attribute__((interrupt)) void divideByZeroHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void debugHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void nonMaskableInterruptHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void breakpointHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void overflowHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void boundRangeExceededHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void invalidOpcodeHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void deviceNotAvailableHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void doubleFaultHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void invalidTSSHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void segmentNotPresentHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void stackSegmentFaultHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void generalProtectionFaultHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void pageFaultHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void x87FloatingPointExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void alignmentCheckHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void machineCheckHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void simdFloatingPointExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void virtualizationExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void controlProtectionExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void hypervisorInjectionExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void vmmCommunicationExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void securityExceptionHandler(struct InterruptFrame *frame);
    __attribute__((interrupt)) void syscallHandler(struct InterruptFrame *frame);
}
