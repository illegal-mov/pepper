#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "RelocationDir.h"

using namespace Pepper;

RelocationBlock::RelocationBlock(const FileBytes &fbytes, int raw)
: IHeader(fbytes, raw)
, m_relocBase(fbytes, raw)
, m_relocTable()
{
    int size = m_relocBase.base()->BlockSize;
    m_relocTable = RelocationTable(fbytes,
        raw + (int)sizeof(IMAGE_BASE_RELOCATION),
        (size - (int)sizeof(IMAGE_BASE_RELOCATION)) / (int)sizeof(IMAGE_BASE_RELOCATION_ENTRY));
}

RelocationDir::RelocationDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        int32_t bytesRead = 0;
        while (bytesRead < dde.size()) {
            m_elements.emplace_back(fbytes, dirOffset() + bytesRead);
            PIMAGE_BASE_RELOCATION tmp = (PIMAGE_BASE_RELOCATION)&dir()[bytesRead];
            /* `BlockSize` typically includes the size of the header,
             * but some silly files might have a null .reloc section
             * with a non-zero RelocDirEntry.size().
             * Add min(sizeof(IMAGE_BASE_RELOCATION), BlockSize)
             * to avoid an infinite loop.
             */
            bytesRead += (tmp->BlockSize < (int)sizeof(IMAGE_BASE_RELOCATION))
                ? (int)sizeof(IMAGE_BASE_RELOCATION)
                : tmp->BlockSize;
        }
    }
}

const char* RelocationTable::getFieldName(int index)
{
    switch (index) {
        case OFFSET: return "Offset";
        case TYPE  : return "Type";
        default    : return "<UNKNOWN>";
    }
}

const char* RelocationTable::getRelocationTypeName(const PeFile &pe, int index)
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
            if      (Ident::isMips(pe))     return "MIPS Jump";
            else if (Ident::isArmThumb(pe)) return "ARM MOV";
            else if (Ident::isRiscv(pe))    return "RISC-V High";
            else                            break;
        case RESERVED_6    : return "<Reserved>";
        case THUMB_MOV32   :
//      case RISCV_LOW12I  : // no duplicate enum values allowed
            if      (Ident::isThumb(pe)) return "Thumb MOV";
            else if (Ident::isRiscv(pe)) return "RISC-V High";
            else                         break;
        case RISCV_LOW12S  : return "RISC-V Low (S-Type)";
        case MIPS_JMPADDR16: return "MIPS16 Jump";
        case DIR64         : return "64-bit HIGHLOW";
    }
    return "<UNKNOWN>";
}

const void* RelocationTable::getFieldPtr(int index) const
{
    return (0 <= index && index < length())
    ? &relocations()[index]
    : nullptr;
}

const char* RelocationBase::getFieldName(int index)
{
    switch (index) {
        case PAGE_RVA  : return "Pointer to Page";
        case BLOCK_SIZE: return "Block Size";
        default        : return "<UNKNOWN>";
    }
}

const void* RelocationBase::getFieldPtr(int index) const
{
    switch (index) {
        case PAGE_RVA  : return &base()->PageRVA;
        case BLOCK_SIZE: return &base()->BlockSize;
        default        : return nullptr;
    }
}

const char* RelocationBlock::getFieldName(int index)
{
    switch (index) {
        case HEAD : return "Base Relocation";
        case TABLE: return "Relocation Entry";
        default   : return "<UNKNOWN>";
    }
}

const void* RelocationBlock::getFieldPtr(int index) const
{
    switch (index) {
        case HEAD : return &m_relocBase;
        case TABLE: return &m_relocTable;
        default   : return nullptr;
    }
}

const char* RelocationDir::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* RelocationDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

