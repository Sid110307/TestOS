#include "include/kernel.h"

extern "C" void main(BootInfo *bootInfo)
{
    auto kernel = initKernel(bootInfo);

    kernel.renderer->clear(Colors::Black);
    kernel.renderer->putString("Hello, World!", Colors::White);

    int *test = reinterpret_cast<int *>(0x4206942042069420);
    *test = 69;

    while (true);
}
