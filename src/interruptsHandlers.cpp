#include "include/interruptHandlers.h"

__attribute__((interrupt)) void Interrupts::pageFaultHandler(InterruptFrame *frame)
{
    globalRenderer->clear(Colors::Blue);
    globalRenderer->putString("Page fault :(", Colors::Red);

    globalRenderer->putString("\n\nProtection violation - ", Colors::Red);
    if (frame->cpuFlags & 0x2) globalRenderer->putString("read", Colors::Red);
    else globalRenderer->putString("write", Colors::Red);
    globalRenderer->putString(" fault - ", Colors::Red);

    size_t cr2;
    asm ("mov %%cr2, %0" : "=r"(cr2));
    globalRenderer->putString(toString(cr2), Colors::Red);

    globalRenderer->putString("\nAddress: ", Colors::Red);
    globalRenderer->putString(toString(cr2), Colors::Red);

    globalRenderer->putString("\nError code: ", Colors::Red);
    globalRenderer->putString(toString(frame->stackPointer), Colors::Red);

    while (true);
}
