#include "FileHdr.h"

using namespace Pepper;

const char* FileHeader::getFieldName(const int index)
{
    switch (index) {
        case MACHINE                : return "Machine";              // type of cpu the binary was compiled for
        case NUMBER_OF_SECTIONS     : return "Number of Sections";   // number of section headers
        case TIMESTAMP              : return "Timestamp";            // file creation time as UNIX epoch
        case POINTER_TO_SYMBOL_TABLE: return "Pointer to Symbols";   // file offset to symbol table (deprecated)
        case NUMBER_OF_SYMBOLS      : return "Number of Symbols";    // number of symbol table entries (deprecated)
        case SIZE_OF_OPTIONAL_HEADER: return "Optional Header Size"; // size of optional header in bytes
        case CHARACTERISTICS        : return "Characteristics";      // bitfields for various things
        default                     : return "<UNKNOWN>";
    }
}

const void* FileHeader::getFieldPtr(const int index) const
{
    switch (index) {
        case MACHINE                : return &getStructPtr()->Machine;              // type of cpu the binary was compiled for
        case NUMBER_OF_SECTIONS     : return &getStructPtr()->NumberOfSections;     // number of section headers
        case TIMESTAMP              : return &getStructPtr()->TimeDateStamp;        // file creation time as UNIX epoch
        case POINTER_TO_SYMBOL_TABLE: return &getStructPtr()->PointerToSymbolTable; // file offset to symbol table (deprecated)
        case NUMBER_OF_SYMBOLS      : return &getStructPtr()->NumberOfSymbols;      // number of symbol table entries (deprecated)
        case SIZE_OF_OPTIONAL_HEADER: return &getStructPtr()->SizeOfOptionalHeader; // size of optional header in bytes
        case CHARACTERISTICS        : return &getStructPtr()->Characteristics;      // bitfields for various things
        default                     : return nullptr;
    }
}

const char* FileHeader::getMachineName(const int id)
{
    switch (id) {
        case AM33     : return "Matsushita AM33";
        case AMD64    : return "x64";
        case ARM      : return "ARM little endian";
        case ARM64    : return "ARM64 little endian";
        case ARMNT    : return "ARM Thumb-2 little endian";
        case EBC      : return "EFI byte code";
        case I386     : return "Intel 386 or later";
        case IA64     : return "Intel Itanium";
        case M32R     : return "Mitsubishi M32R little endian";
        case MIPS16   : return "MIPS16";
        case MIPSFPU  : return "MIPS with FPU";
        case MIPSFPU16: return "MIPS16 with FPU";
        case POWERPC  : return "Power PC little endian";
        case POWERPCFP: return "Power PC with floating point";
        case R4000    : return "MIPS little endian";
        case RISCV32  : return "RISC-V 32-bit";
        case RISCV64  : return "RISC-V 64-bit";
        case RISCV128 : return "RISC-V 128-bit";
        case SH3      : return "Hitachi SH3";
        case SH3DSP   : return "Hitachi SH3 DSP";
        case SH4      : return "Hitachi SH4";
        case SH5      : return "Hitachi SH5";
        case THUMB    : return "Thumb";
        case WCEMIPSV2: return "MIPS WCEv2 little endian";
        default       : return "<UNKNOWN>";
    }
}

const char* FileHeader::getCharacteristicName(const int index)
{
    switch (index) {
        case RELOCS_STRIPPED        : return "Relocations Stripped";
        case EXECUTABLE_IMAGE       : return "Image is Executable";
        case LINE_NUMS_STRIPPED     : return "Line Numbers Stripped";
        case LOCAL_SYMS_STRIPPED    : return "Local Symbols Stripped";
        case AGGRESIVE_WS_TRIM      : return "Trimmed Working Set";
        case LARGE_ADDRESS_AWARE    : return "Large Address Support";
        case RESERVED_40            : return "<Reserved>";
        case BYTES_REVERSED_LO      : return "Little Endian";
        case IS_32BIT_MACHINE       : return "Machine is 32 Bit";
        case DEBUG_STRIPPED         : return "Debug Info Stripped";
        case REMOVABLE_RUN_FROM_SWAP: return "Removable Media to Swap";
        case NET_RUN_FROM_SWAP      : return "Network Media to Swap";
        case SYSTEM                 : return "File is System File";
        case DLL                    : return "File is DLL";
        case UP_SYSTEM_ONLY         : return "No Multiprocessor";
        case BYTES_REVERSED_HI      : return "Big Endian";
        default                     : return "<UNKNOWN>";
    }
}

