#include "include/interrupts.h"

void IDTEntry::setOffset(size_t offset)
{
    offsetLow = offset & 0xFFFF;
    offsetMid = offset >> 16 & 0xFFFF;
    offsetHigh = offset >> 32 & 0xFFFFFFFF;
}

size_t IDTEntry::getOffset() const
{
    return static_cast<size_t>(offsetLow) | static_cast<size_t>(offsetMid) << 16 |
           static_cast<size_t>(offsetHigh) << 32;
}

IDTEntry InterruptManager::idt[IDT_SIZE];
void (*InterruptManager::handlers[IDT_SIZE])(InterruptFrame *frame);

void InterruptManager::initialize()
{
    IDTDescriptor idtDescriptor = {sizeof(IDTEntry) * IDT_SIZE - 1, reinterpret_cast<size_t>(&idt)};
    Memory::memset(idt, 0, sizeof(IDTEntry) * IDT_SIZE);

    registerHandler(IRQ0, []([[maybe_unused]] InterruptFrame *frame) { outPort(PIC1_COMMAND, PIC_EOI); });
    registerHandler(IRQ1, []([[maybe_unused]] InterruptFrame *frame) { outPort(PIC1_COMMAND, PIC_EOI); });
    registerHandler(IRQ12, []([[maybe_unused]] InterruptFrame *frame) { outPort(PIC2_COMMAND, PIC_EOI); });

    outPort(PIC1_COMMAND, 0x11);
    outPort(PIC2_COMMAND, 0x11);
    outPort(PIC1_DATA, IRQ0);
    outPort(PIC2_DATA, IRQ1);
    outPort(PIC1_DATA, 0x04);
    outPort(PIC2_DATA, 0x02);
    outPort(PIC1_DATA, 0x01);
    outPort(PIC2_DATA, 0x01);
    outPort(PIC1_DATA, 0x00);
    outPort(PIC2_DATA, 0x00);

    registerHandler(0x80, []([[maybe_unused]] InterruptFrame *frame) {});

    asm volatile("lidt %0" : : "m"(idtDescriptor));
    enableInterrupts();
}

void InterruptManager::registerHandler(unsigned char interrupt, void (*handler)(InterruptFrame *frame))
{
    handlers[interrupt] = handler;

    IDTEntry &entry = idt[interrupt];
    entry.selector = KERNEL_CS;
    entry.ist = 0;
    entry.typeAttr = 0x8E;
    entry.setOffset(reinterpret_cast<size_t>(handler));
    entry.zero = 0;
}

void InterruptManager::enableInterrupts() { asm volatile("sti"); }
void InterruptManager::disableInterrupts() { asm volatile("cli"); }

void InterruptManager::outPort(unsigned short port,
                               unsigned char data) { asm volatile("outb %0, %1" : : "a"(data), "Nd"(port)); }

unsigned char InterruptManager::inPort(unsigned short port)
{
    unsigned char data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));

    return data;
}

void InterruptManager::interruptHandler(InterruptFrame *frame)
{
    if (handlers[frame->instructionPointer] != nullptr) handlers[frame->instructionPointer](frame);
    else
    {
        globalRenderer.putString("Unhandled interrupt: ", Colors::Red);
        globalRenderer.putNumber(frame->instructionPointer, Colors::Red);
        globalRenderer.putString("\n", Colors::Red);
    }
}
