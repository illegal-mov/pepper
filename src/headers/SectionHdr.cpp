#include "SectionHdr.h"

#include "FileHdr.h"

using namespace Pepper;

SectionHeaders::SectionHeaders(const FileBytes& fbytes, const FileHeader& file)
{
    // find base of section headers
    const uint16_t optHdrSize = file.getStructPtr()->SizeOfOptionalHeader;
    const size_t base = file.hdrOffset() + sizeof(IMAGE_FILE_HEADER) + optHdrSize;

    // construct each section header
    const uint16_t length = file.getStructPtr()->NumberOfSections;
    for (size_t i=0; i < length; i++) {
        m_sectionHeaders.emplace_back(fbytes, base + (i * sizeof(IMAGE_SECTION_HEADER)));
    }
}

const char* SectionHeaderEntry::getFieldName(const int index)
{
    switch (index) {
        case NAME                   : return "Name";
        case VIRTUAL_SIZE           : return "Virtual Size";
        case VIRTUAL_ADDRESS        : return "Virtual Address";
        case SIZE_OF_RAW_DATA       : return "Size of Raw Data";
        case POINTER_TO_RAW_DATA    : return "Pointer to Raw Data";
        case POINTER_TO_RELOCATIONS : return "Pointer to Relocations";
        case POINTER_TO_LINE_NUMBERS: return "Pointer to Line Numbers";
        case NUMBER_OF_RELOCATIONS  : return "Number of Relocations";
        case NUMBER_OF_LINE_NUMBERS : return "Number of Line Numbers";
        case CHARACTERISTICS        : return "Characteristics";
        default                     : return "<UNKNOWN>";
    }
}

const void* SectionHeaderEntry::getFieldPtr(const int index) const
{
    switch (index) {
        case NAME                   : return &getStructPtr()->Name;
        case VIRTUAL_SIZE           : return &getStructPtr()->VirtualSize;
        case VIRTUAL_ADDRESS        : return &getStructPtr()->VirtualAddress;
        case SIZE_OF_RAW_DATA       : return &getStructPtr()->SizeOfRawData;
        case POINTER_TO_RAW_DATA    : return &getStructPtr()->PointerToRawData;
        case POINTER_TO_RELOCATIONS : return &getStructPtr()->PointerToRelocations;
        case POINTER_TO_LINE_NUMBERS: return &getStructPtr()->PointerToLineNumbers;
        case NUMBER_OF_RELOCATIONS  : return &getStructPtr()->NumberOfRelocations;
        case NUMBER_OF_LINE_NUMBERS : return &getStructPtr()->NumberOfLineNumbers;
        case CHARACTERISTICS        : return &getStructPtr()->Characteristics;
        default                     : return nullptr;
    }
}

const char* SectionHeaderEntry::getCharacteristicName(const int index)
{
    // must cast to unsigned because of so many fields
    switch (static_cast<unsigned int>(index)) {
        case RESERVED_1     : return "<Reserved>";
        case RESERVED_2     : return "<Reserved>";
        case RESERVED_4     : return "<Reserved>";
        case TYPE_NO_PAD    : return "Not Padded";
        case RESERVED_10    : return "<Reserved>";
        case CNT_CODE       : return "Contains Executable Code";
        case CNT_INIT       : return "Contains Initialized Data";
        case CNT_UNINIT     : return "Contains Uninitialized Data";
        case RESERVED_100   : return "<Reserved>";
        case LNK_INFO       : return "Contains Miscellaneous Information";
        case RESERVED_400   : return "<Reserved>";
        case LNK_REMOVE     : return "Not Part of Image";
        case LNK_COMDAT     : return "Contains COMDAT Data";
        case RESERVED_2000  : return "<Reserved>";
        case RESERVED_4000  : return "<Reserved>";
        case GPREL          : return "Referenced by Global Pointer";
        case MEM_PURGEABLE  : return "<Reserved>";
        case MEM_16BIT      : return "<Reserved>";
        case MEM_LOCKED     : return "<Reserved>";
        case MEM_PRELOAD    : return "<Reserved>";
        case LNK_NRELOC_OVFL: return "Contains Extended Relocations";
        case MEM_DISCARDABLE: return "Discardable";
        case MEM_NOT_CACHED : return "Uncacheable";
        case MEM_NOT_PAGED  : return "Unpageable";
        case MEM_SHARED     : return "Sharable";
        case MEM_EXECUTE    : return "Executable";
        case MEM_READ       : return "Readable";
        case MEM_WRITE      : return "Writable";
        default             : return "<UNKNOWN>";
    }
}

const char* SectionHeaderEntry::getCharacteristicAlignName(const int alignNybble)
{
    switch (alignNybble) {
        case ALIGN_1BYTES   : return "1-byte Alignment";
        case ALIGN_2BYTES   : return "2-byte Alignment";
        case ALIGN_4BYTES   : return "4-byte Alignment";
        case ALIGN_8BYTES   : return "8-byte Alignment";
        case ALIGN_16BYTES  : return "16-byte Alignment";
        case ALIGN_32BYTES  : return "32-byte Alignment";
        case ALIGN_64BYTES  : return "64-byte Alignment";
        case ALIGN_128BYTES : return "128-byte Alignment";
        case ALIGN_256BYTES : return "256-byte Alignment";
        case ALIGN_512BYTES : return "512-byte Alignment";
        case ALIGN_1024BYTES: return "1024-byte Alignment";
        case ALIGN_2048BYTES: return "2048-byte Alignment";
        case ALIGN_4096BYTES: return "4096-byte Alignment";
        case ALIGN_8192BYTES: return "8192-byte Alignment";
        default             : return "<UNKNOWN>";
    }
}

const char* SectionHeaders::getFieldName(const int index)
{
    return SectionHeaderEntry::getFieldName(index);
}

const char* SectionHeaders::getCharacteristicName(const int index)
{
    return SectionHeaderEntry::getCharacteristicName(index);
}

const char* SectionHeaders::getCharacteristicAlignName(const int index)
{
    return SectionHeaderEntry::getCharacteristicAlignName(index);
}

