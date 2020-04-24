#ifndef RELOCATION_H
#define RELOCATION_H

#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"

/*
typedef struct _IMAGE_BASE_RELOCATION {
    uint32_t PageRVA;
    uint32_t BlockSize;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_BASE_RELOCATION_ENTRY {
    uint16_t Offset : 12;
    uint16_t Type   : 4;
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

    RelocationTable() = default;

    RelocationTable(const FileBytes& fbytes, const offset_t raw, const size_t len)
    : IHeader(fbytes, raw)
    , m_length(len)
    {}

    const void* getFieldPtr(const int index) const override;

    const IMAGE_BASE_RELOCATION_ENTRY* getStructPtr(const int index) const
    {
        return static_cast<const IMAGE_BASE_RELOCATION_ENTRY*>(getFieldPtr(index));
    }

    int16_t type(const int index) const { return getStructPtr()[index].Type & 0xF; }
    int16_t offset(const int index) const { return getStructPtr()[index].Offset & 0x0FFF; }
    size_t length() const { return m_length; }

    static const char* getFieldName(const int index);
    static const char* getRelocationTypeName(const PeFile& pe, const int index);

private:
    size_t m_length{};
    const IMAGE_BASE_RELOCATION_ENTRY* getStructPtr() const { return static_cast<const IMAGE_BASE_RELOCATION_ENTRY*>(hdr()); }
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

    RelocationBase(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    const void* getFieldPtr(const int index) const override;
    const IMAGE_BASE_RELOCATION* getStructPtr() const { return static_cast<const IMAGE_BASE_RELOCATION*>(hdr()); }

    static const char* getFieldName(const int index);
};

/* A RelocationBlock is an IMAGE_BASE_RELOCATION structure immediately
 * followed by a variable number of IMAGE_BASE_RELOCATION_ENTRY structures.
 * The number of IMAGE_BASE_RELOCATION_ENTRY in a block is given by the formula
 * (IMAGE_BASE_RELOCATION.BlockSize - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_BASE_RELOCATION_ENTRY)
 */
class RelocationBlock final : public IHeader {
public:
    enum Fields {
        HEAD,
        TABLE,
        _NUM_FIELDS,
    };

    RelocationBlock(const FileBytes& fbytes, const offset_t raw);

    const void* getFieldPtr(const int index) const override;
    const RelocationBase*  relocBase()  const { return &m_relocBase; }
    const RelocationTable* relocTable() const { return &m_relocTable; }

    static const char* getFieldName(const int index);

private:
    RelocationBase  m_relocBase;
    RelocationTable m_relocTable;
};

/* Variable-length array of RelocationBlocks
 */
class RelocationDir final : public IDirectory {
public:
    RelocationDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const void* getFieldPtr(const int index) const override;
    const std::vector<RelocationBlock>& blocks() const { return m_relocationBlocks; }
    size_t length() const { return m_relocationBlocks.size(); }

    static const char* getFieldName(const int index);

private:
    std::vector<RelocationBlock> m_relocationBlocks{};
};
} // namespace Pepper

#endif
