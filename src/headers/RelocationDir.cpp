#include "RelocationDir.h"

#include "../Identification.h"
#include "../Types.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

RelocationBlock::RelocationBlock(const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
, m_relocBase(fbytes, raw)
, m_relocTable()
{
    const uint32_t size = m_relocBase.getStructPtr()->BlockSize;
    m_relocTable = RelocationTable(fbytes,
        raw + sizeof(IMAGE_BASE_RELOCATION),
        (size - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_BASE_RELOCATION_ENTRY));
}

RelocationDir::RelocationDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        size_t bytesRead = 0;
        while (bytesRead < dde.size()) {
            m_relocationBlocks.emplace_back(fbytes, dirOffset() + bytesRead);
            const void *base = &static_cast<const char*>(dir())[bytesRead];
            const IMAGE_BASE_RELOCATION* tmp = static_cast<const IMAGE_BASE_RELOCATION*>(base);
            /* `BlockSize` typically includes the size of the header,
             * but some silly files might have a null .reloc section
             * with a non-zero RelocDirEntry.size().
             * Add min(sizeof(IMAGE_BASE_RELOCATION), BlockSize)
             * to avoid an infinite loop.
             */
            bytesRead += (tmp->BlockSize < sizeof(IMAGE_BASE_RELOCATION))
                ? sizeof(IMAGE_BASE_RELOCATION)
                : tmp->BlockSize;
        }
    }
}

const char* RelocationTable::getFieldName(const int index)
{
    switch (index) {
        case OFFSET: return "Offset";
        case TYPE  : return "Type";
        default    : return "<UNKNOWN>";
    }
}

const char* RelocationTable::getRelocationTypeName(const PeFile& pe, const int index)
{
    switch (index) {
        case ABSOLUTE      : return "Absolute";
        case HIGH          : return "High";
        case LOW           : return "Low";
        case HIGHLOW       : return "High and Low";
        case HIGHADJ       : return "High Adjacent";
        case MIPS_JMPADDR  :
//      case ARM_MOV32     : // no duplicate enum values allowed
//      case RISCV_HIGH20  : // no duplicate enum values allowed
            if      (Ident::isMachineTypeMips(pe))     return "MIPS Jump";
            else if (Ident::isMachineTypeArmThumb(pe)) return "ARM MOV";
            else if (Ident::isMachineTypeRiscv(pe))    return "RISC-V High";
            else                            break;
        case RESERVED_6    : return "<Reserved>";
        case THUMB_MOV32   :
//      case RISCV_LOW12I  : // no duplicate enum values allowed
            if      (Ident::isMachineTypeThumb(pe)) return "Thumb MOV";
            else if (Ident::isMachineTypeRiscv(pe)) return "RISC-V High";
            else                         break;
        case RISCV_LOW12S  : return "RISC-V Low (S-Type)";
        case MIPS_JMPADDR16: return "MIPS16 Jump";
        case DIR64         : return "64-bit HIGHLOW";
    }
    return "<UNKNOWN>";
}

const void* RelocationTable::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
    ? &getStructPtr()[uindex]
    : nullptr;
}

const char* RelocationBase::getFieldName(const int index)
{
    switch (index) {
        case PAGE_RVA  : return "Pointer to Page";
        case BLOCK_SIZE: return "Block Size";
        default        : return "<UNKNOWN>";
    }
}

const void* RelocationBase::getFieldPtr(const int index) const
{
    switch (index) {
        case PAGE_RVA  : return &getStructPtr()->PageRVA;
        case BLOCK_SIZE: return &getStructPtr()->BlockSize;
        default        : return nullptr;
    }
}

const char* RelocationBlock::getFieldName(const int index)
{
    switch (index) {
        case HEAD : return "Base Relocation";
        case TABLE: return "Relocation Entry";
        default   : return "<UNKNOWN>";
    }
}

const void* RelocationBlock::getFieldPtr(const int index) const
{
    switch (index) {
        case HEAD : return &m_relocBase;
        case TABLE: return &m_relocTable;
        default   : return nullptr;
    }
}

const char* RelocationDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* RelocationDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

