#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct
{
    unsigned char magic[2], mode, charSize;
} PSF1Header;

typedef struct
{
    PSF1Header *fontHeader;
    void *glyphBuffer;
} PSF1Font;

typedef struct
{
    unsigned int *baseAddress;
    unsigned int bufferSize;
    unsigned int width;
    unsigned int height;
    unsigned int pixelsPerScanLine;
} Framebuffer;

typedef struct
{
    PSF1Font *font;
    Framebuffer *framebuffer;
} BootInfo;

Framebuffer framebuffer;

EFI_FILE *loadFile(EFI_FILE *dir, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    EFI_FILE *loadedFile;
    EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *filesystem;

    systemTable->BootServices->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void **) &loadedImage);
    systemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid,
                                              (void **) &filesystem);
    if (dir == NULL) filesystem->OpenVolume(filesystem, &dir);

    EFI_STATUS status = dir->Open(dir, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS)
    {
        Print(L"Could not load file \"%s\"!\r\n", path);
        return NULL;
    }

    return loadedFile;
}

PSF1Font *loadFont(EFI_FILE *dir, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    EFI_FILE *font = loadFile(dir, path, imageHandle, systemTable);
    if (font == NULL)
    {
        Print(L"Could not load font \"%s\"!\r\n", path);
        return NULL;
    }

    PSF1Header *fontHeader;
    {
        UINTN size = sizeof(PSF1Header);

        systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void **) &fontHeader);
        font->Read(font, &size, fontHeader);
    }

    if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
    {
        Print(L"Bad font magic number!\r\n");
        return NULL;
    }

    UINTN glyphBufferSize = fontHeader->charSize * 256;
    if (fontHeader->mode == 1) glyphBufferSize = fontHeader->charSize * 512;
    void *glyphBuffer;
    {
        font->SetPosition(font, sizeof(PSF1Header));
        systemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void **) &glyphBuffer);
        font->Read(font, &glyphBufferSize, glyphBuffer);
    }

    PSF1Font *loadedFont;
    {
        systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1Font), (void **) &loadedFont);
        loadedFont->fontHeader = fontHeader;
        loadedFont->glyphBuffer = glyphBuffer;
    }

    return loadedFont;
}

Framebuffer *initializeGOP()
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_STATUS status;

    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void **) &gop);
    if (EFI_ERROR(status))
    {
        Print(L"Could not load GOP!\r\n");
        return NULL;
    }

    framebuffer.baseAddress = (unsigned int *) gop->Mode->FrameBufferBase;
    framebuffer.bufferSize = gop->Mode->FrameBufferSize;
    framebuffer.width = gop->Mode->Info->HorizontalResolution;
    framebuffer.height = gop->Mode->Info->VerticalResolution;
    framebuffer.pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

    return &framebuffer;
}

int memcmp(const void *aPtr, const void *bPtr, size_t n)
{
    const unsigned char *a = aPtr, *b = bPtr;
    for (size_t i = 0; i < n; ++i) if (a[i] != b[i]) return a[i] - b[i];

    return 0;
}

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    InitializeLib(imageHandle, systemTable);
    Print(L"Loading kernel... ");

    EFI_FILE *kernel = loadFile(NULL, L"kernel.elf", imageHandle, systemTable);
    if (kernel == NULL) Print(L"Could not load kernel!\r\n");

    Elf64_Ehdr header;
    {
        UINTN fileInfoSize;
        EFI_FILE_INFO *fileInfo;
        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
        systemTable->BootServices->AllocatePool(EfiLoaderData, fileInfoSize, (void **) &fileInfo);
        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, (void **) &fileInfo);

        UINTN size = sizeof(header);
        kernel->Read(kernel, &size, &header);
    }

    if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 || header.e_ident[EI_CLASS] != ELFCLASS64 ||
        header.e_ident[EI_DATA] != ELFDATA2LSB || header.e_type != ET_EXEC || header.e_machine != EM_X86_64 ||
        header.e_version != EV_CURRENT)
        Print(L"Bad kernel header format!\r\n");

    Elf64_Phdr *programHeaders;
    {
        kernel->SetPosition(kernel, header.e_phoff);
        UINTN size = header.e_phnum * header.e_phentsize;
        systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void **) &programHeaders);
        kernel->Read(kernel, &size, programHeaders);
    }

    for (Elf64_Phdr *programHeader = programHeaders;
         (char *) programHeader < (char *) programHeaders + header.e_phnum * header.e_phentsize;
         programHeader = (Elf64_Phdr *) ((char *) programHeader + header.e_phentsize))
        switch (programHeader->p_type)
        {
            case PT_LOAD:
            {
                int pages = ((int) programHeader->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = programHeader->p_paddr;
                systemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

                kernel->SetPosition(kernel, programHeader->p_offset);
                UINTN size = programHeader->p_filesz;
                kernel->Read(kernel, &size, (void *) segment);

                break;
            }
            default:
                break;
        }

    Print(L"Kernel loaded successfully!\r\n");

    PSF1Font *font = loadFont(NULL, L"zap-ext-light18.psf", imageHandle, systemTable);
    if (font == NULL) Print(L"Could not load font!\r\n");

    framebuffer = *initializeGOP();
    if (framebuffer.bufferSize == 0) Print(L"Could not initialize GOP!\r\n");

    BootInfo bootInfo;
    bootInfo.font = font;
    bootInfo.framebuffer = &framebuffer;

    ((__attribute__((sysv_abi)) int (*)(BootInfo *)) header.e_entry)(&bootInfo);
    return EFI_SUCCESS;
}
