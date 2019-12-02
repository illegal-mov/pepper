#ifndef SECTION_H
#define SECTION_H

#include <vector>

#include "../generics/iHdr.h"
#include "struct.h"

/*
typedef struct _IMAGE_SECTION_HEADER {
    char    Name[8];              // section name as ASCII string
    int32_t VirtualSize;          // section size in memory
    int32_t VirtualAddress;       // address of section in memory
    int32_t SizeOfRawData;        // section size on disk
    int32_t PointerToRawData;     // offset of section on disk
    int32_t PointerToRelocations; // offset to relocations on disk
    int32_t PointerToLineNumbers; // offset to line numbers on disk
    int16_t NumberOfRelocations;  // number of relocation entries
    int16_t NumberOfLineNumbers;  // number of line number entries
    int32_t Characteristics;      // bit fields for various properties
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
*/

namespace Pepper {

// forward declarations
class FileHeader;

/* SectionHeaderEntry is class wrapping an entry in the SectionHeaders array.
 */
class SectionHeaderEntry final : public IHeader {
public:
    enum Fields {
        NAME,
        VIRTUAL_SIZE,
        VIRTUAL_ADDRESS,
        SIZE_OF_RAW_DATA,
        POINTER_TO_RAW_DATA,
        POINTER_TO_RELOCATIONS,
        POINTER_TO_LINE_NUMBERS,
        NUMBER_OF_RELOCATIONS,
        NUMBER_OF_LINE_NUMBERS,
        CHARACTERISTICS,
        _NUM_FIELDS,
    };

    enum Characteristics {
        RESERVED_1      = 0x00000001,
        RESERVED_2      = 0x00000002,
        RESERVED_4      = 0x00000004,
        TYPE_NO_PAD     = 0x00000008,
        RESERVED_10     = 0x00000010,
        CNT_CODE        = 0x00000020,
        CNT_INIT        = 0x00000040,
        CNT_UNINIT      = 0x00000080,
        RESERVED_100    = 0x00000100,
        LNK_INFO        = 0x00000200,
        RESERVED_400    = 0x00000400,
        LNK_REMOVE      = 0x00000800,
        LNK_COMDAT      = 0x00001000,
        RESERVED_2000   = 0x00002000,
        RESERVED_4000   = 0x00004000,
        GPREL           = 0x00008000,
        MEM_PURGEABLE   = 0x00010000,
        MEM_16BIT       = 0x00020000,
        MEM_LOCKED      = 0x00040000,
        MEM_PRELOAD     = 0x00080000,
        ALIGN_1BYTES    = 0x00100000,
        ALIGN_2BYTES    = 0x00200000,
        ALIGN_4BYTES    = 0x00300000,
        ALIGN_8BYTES    = 0x00400000,
        ALIGN_16BYTES   = 0x00500000,
        ALIGN_32BYTES   = 0x00600000,
        ALIGN_64BYTES   = 0x00700000,
        ALIGN_128BYTES  = 0x00800000,
        ALIGN_256BYTES  = 0x00900000,
        ALIGN_512BYTES  = 0x00A00000,
        ALIGN_1024BYTES = 0x00B00000,
        ALIGN_2048BYTES = 0x00C00000,
        ALIGN_4096BYTES = 0x00D00000,
        ALIGN_8192BYTES = 0x00E00000,
        LNK_NRELOC_OVFL = 0x01000000,
        MEM_DISCARDABLE = 0x02000000,
        MEM_NOT_CACHED  = 0x04000000,
        MEM_NOT_PAGED   = 0x08000000,
        MEM_SHARED      = 0x10000000,
        MEM_EXECUTE     = 0x20000000,
        MEM_READ        = 0x40000000,
        MEM_WRITE       = 0x80000000,
    };

    SectionHeaderEntry(const FileBytes &fbytes, int offset)
    : IHeader(fbytes, offset)
    {}

    // member functions
    const void* getFieldPtr(int index) const override;
    const IMAGE_SECTION_HEADER* entry() const { return (IMAGE_SECTION_HEADER*)hdr(); }

    // static functions
    static const char* getFieldName(int index);
    static const char* getCharacteristicName(int index);
    static const char* getCharacteristicAlignName(int alignNybble);
};

/* Section headers exist immediately after the Optional header.
 * Section headers are stored as an array whose length is given by
 * FileHeader.NumberOfSections
 */
class SectionHeaders final {
private:
    std::vector<SectionHeaderEntry> m_elements{};
public:
    static constexpr int ALIGN_NYBBLE = 0x00F00000;

    SectionHeaders(const FileBytes &fbytes, const FileHeader &file);

    // member functions
    const std::vector<SectionHeaderEntry>& sections() const { return m_elements; }
    int length() const { return (int)m_elements.size(); }

    // static functions
    static const char* getFieldName(int index);
    static const char* getCharacteristicName(int index);
    static const char* getCharacteristicAlignName(int index);
};
} // namespace Pepper

#endif
