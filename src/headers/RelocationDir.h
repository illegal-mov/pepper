#ifndef RELOCATION_H
#define RELOCATION_H

#include <vector>

#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_BASE_RELOCATION {
    int32_t PageRVA;
    int32_t BlockSize;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_BASE_RELOCATION_ENTRY {
    int16_t Offset : 12;
    int16_t Type   : 4;
} IMAGE_BASE_RELOCATION_ENTRY, *PIMAGE_BASE_RELOCATION_ENTRY;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* Variable-length array of IMAGE_BASE_RELOCATION_ENTRY
 */
class RelocationTable final : public IHeader {
private:
    int m_length{};
    const IMAGE_BASE_RELOCATION_ENTRY* relocations() const { return (IMAGE_BASE_RELOCATION_ENTRY*)hdr(); }
public:
    enum Fields {
        OFFSET,
        TYPE,
        _NUM_FIELDS,
    };

    enum Types {
        ABSOLUTE       = 0x0,
        HIGH           = 0x1,
        LOW            = 0x2,
        HIGHLOW        = 0x3,
        HIGHADJ        = 0x4,
        MIPS_JMPADDR   = 0x5,
        ARM_MOV32      = 0x5,
        RISCV_HIGH20   = 0x5,
        RESERVED_6     = 0x6,
        THUMB_MOV32    = 0x7,
        RISCV_LOW12I   = 0x7,
        RISCV_LOW12S   = 0x8,
        MIPS_JMPADDR16 = 0x9,
        DIR64          = 0xA,
    };

    RelocationTable()
    : IHeader()
    {}

    RelocationTable(const FileBytes &fbytes, size_t raw, int len)
    : IHeader(fbytes, raw)
    , m_length(len)
    {}

    // member functions
    const void* getFieldPtr(int index) const override;

    PIMAGE_BASE_RELOCATION_ENTRY entry(int index) const
    {
        return (PIMAGE_BASE_RELOCATION_ENTRY)getFieldPtr(index);
    }

    int16_t type(int index) const { return relocations()[index].Type & 0xF; }
    int16_t offset(int index) const { return relocations()[index].Offset & 0x0FFF; }
    int length() const { return m_length; }

    // static functions
    static const char* getFieldName(int index);
    static const char* getRelocationTypeName(const PeFile &pe, int index);
};

/* A single IMAGE_BASE_RELOCATION preceding the variable number of IMAGE_BASE_RELOCATION_ENTRY
 */
class RelocationBase final : public IHeader {
public:
    enum Fields {
        PAGE_RVA,
        BLOCK_SIZE,
        _NUM_FIELDS,
    };

    RelocationBase(const FileBytes &fbytes, size_t raw)
    : IHeader(fbytes, raw)
    {}

    RelocationBase()
    : IHeader()
    {}

    // member functions
    const void* getFieldPtr(int index) const override;
    const IMAGE_BASE_RELOCATION* base() const { return (IMAGE_BASE_RELOCATION*)hdr(); }

    // static functions
    static const char* getFieldName(int index);
};

/* A RelocationBlock is an IMAGE_BASE_RELOCATION structure immediately
 * followed by a variable number of IMAGE_BASE_RELOCATION_ENTRY structures.
 * The number of IMAGE_BASE_RELOCATION_ENTRY in a block is given by the formula
 * (IMAGE_BASE_RELOCATION.BlockSize - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_BASE_RELOCATION_ENTRY)
 */
class RelocationBlock final : public IHeader {
private:
    RelocationBase  m_relocBase;
    RelocationTable m_relocTable;
public:
    enum Fields {
        HEAD,
        TABLE,
        _NUM_FIELDS,
    };

    RelocationBlock()
    : IHeader()
    , m_relocBase()
    , m_relocTable()
    {}

    RelocationBlock(const FileBytes &fbytes, size_t raw);

    // member functions
    const void* getFieldPtr(int index) const override;
    const RelocationBase*  relocBase()  const { return &m_relocBase; }
    const RelocationTable* relocTable() const { return &m_relocTable; }

    // static functions
    static const char* getFieldName(int index);
};

/* Variable-length array of RelocationBlocks
 */
class RelocationDir final : public IDirectory {
private:
    std::vector<RelocationBlock> m_elements{};
public:
    RelocationDir()
    : IDirectory()
    {}

    RelocationDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* getFieldPtr(int index) const override;
    const std::vector<RelocationBlock>& blocks() const { return m_elements; }
    size_t length() const { return m_elements.size(); }

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif
