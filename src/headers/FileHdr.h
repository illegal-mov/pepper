#ifndef FILE_H
#define FILE_H

#include "../Exceptions.h"
#include "../generics/iHdr.h"
#include "DosHdr.h"
#include "struct.h"

/*
typedef struct _IMAGE_FILE_HEADER {
    int16_t Machine;              // type of cpu the binary was compiled for
    int16_t NumberOfSections;     // number of section headers
    int32_t TimeDateStamp;        // file creation time as UNIX epoch
    int32_t PointerToSymbolTable; // file offset to symbol table (deprecated)
    int32_t NumberOfSymbols;      // number of symbol table entries (deprecated)
    int16_t SizeOfOptionalHeader; // size of optional header in bytes
    int16_t Characteristics;      // bitfields for various things
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
*/

namespace Pepper {

/* File header exists 4 bytes after the file offset given by the DOS header's e_lfanew
 * because the 4-byte NT signature appears immediately before the File header.
 */
class FileHeader final : public IHeader {
public:
    enum Fields {
        MACHINE,
        NUMBER_OF_SECTIONS,
        TIMESTAMP,
        POINTER_TO_SYMBOL_TABLE,
        NUMBER_OF_SYMBOLS,
        SIZE_OF_OPTIONAL_HEADER,
        CHARACTERISTICS,
        _NUM_FIELDS,
    };

    enum Machine {
        UNKNOWN   = 0x0000,
        AM33      = 0x01d3,
        AMD64     = 0x8664,
        ARM       = 0x01c0,
        ARM64     = 0xaa64,
        ARMNT     = 0x01c4,
        EBC       = 0x0ebc,
        I386      = 0x014c,
        IA64      = 0x0200,
        M32R      = 0x9041,
        MIPS16    = 0x0266,
        MIPSFPU   = 0x0366,
        MIPSFPU16 = 0x0466,
        POWERPC   = 0x01f0,
        POWERPCFP = 0x01f1,
        R4000     = 0x0166,
        RISCV32   = 0x5032,
        RISCV64   = 0x5064,
        RISCV128  = 0x5128,
        SH3       = 0x01a2,
        SH3DSP    = 0x01a3,
        SH4       = 0x01a6,
        SH5       = 0x01a8,
        THUMB     = 0x01c2,
        WCEMIPSV2 = 0x0169,
    };

    enum Characteristics {
        RELOCS_STRIPPED         = 0x0001,
        EXECUTABLE_IMAGE        = 0x0002,
        LINE_NUMS_STRIPPED      = 0x0004,
        LOCAL_SYMS_STRIPPED     = 0x0008,
        AGGRESIVE_WS_TRIM       = 0x0010,
        LARGE_ADDRESS_AWARE     = 0x0020,
        RESERVED_40             = 0x0040,
        BYTES_REVERSED_LO       = 0x0080,
        IS_32BIT_MACHINE        = 0x0100,
        DEBUG_STRIPPED          = 0x0200,
        REMOVABLE_RUN_FROM_SWAP = 0x0400,
        NET_RUN_FROM_SWAP       = 0x0800,
        SYSTEM                  = 0x1000,
        DLL                     = 0x2000,
        UP_SYSTEM_ONLY          = 0x4000,
        BYTES_REVERSED_HI       = 0x8000,
    };

    FileHeader(const FileBytes &fbytes, const DosHeader &dos)
    : IHeader(fbytes, 0)
    {
        const uint16_t lfanew = dos.dos()->e_lfanew;
        m_baseOffset = static_cast<size_t>(lfanew) + 4; // NT signature is "PE\0\0"

        const int32_t sig = *(int32_t*)ntSig();
        if (sig != 0x00004550 && sig != 0x50450000) {
            throw BadSignature("NT Header magic is not \"PE\"");
        }
    }

    // member functions
    const IMAGE_FILE_HEADER* file() const { return (PIMAGE_FILE_HEADER)hdr(); }
    const void* getFieldPtr(const int index) const override;
    const char* ntSig() const { return &((char*)hdr())[-4]; }

    // static functions
    static const char* getFieldName(const int index);
    static const char* getMachineName(const int id);
    static const char* getCharacteristicName(const int index);
};
} // namespace Pepper

#endif
