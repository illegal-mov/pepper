#ifndef IMPORTSCOMMON_H
#define IMPORTSCOMMON_H

#include <vector>

#include "../Identification.h"
#include "../Types.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_THUNK_DATA32 {
    union {
        struct {
            int32_t OrdinalNumber : 16;
            int32_t Null          : 15; // must be zero
            int32_t OrdinalFlag   : 1;
        };
        int32_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_THUNK_DATA64 {
    union {
        struct {
            int64_t OrdinalNumber : 16;
            int64_t Null          : 47; // must be zero
            int64_t OrdinalFlag   : 1;
        };
        int64_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_IMPORT_BY_NAME {
    int16_t Hint;   // Index into export name table pointer
    char    Name[]; // Imported function name as null-terminated ASCII string
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
*/

namespace Pepper {

class FileBytes;
template <typename ArchType>
class ImportThunk;

using ImportThunk32 = ImportThunk<IMAGE_THUNK_DATA32>;
using ImportThunk64 = ImportThunk<IMAGE_THUNK_DATA64>;

/* A 2-byte hint and imported function name as an ASCII string
 */
class ImportName final : public IHeader {
public:
    enum Fields {
        HINT,
        NAME,
        _NUM_FIELDS,
    };

    ImportName() = default;

    ImportName(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    const void* getFieldPtr(const int index) const override;
    const IMAGE_IMPORT_BY_NAME* getStructPtr() const
    {
        return static_cast<const IMAGE_IMPORT_BY_NAME*>(hdr());
    }

    static const char* getFieldName(const int index);
};

/* An IMAGE_THUNK_DATA struct
 * RVA to ImportName as int32_t or int64_t
 */
template <typename ArchType>
class ImportThunk final : public IHeader {
public:
    enum Fields {
        HINTNAMERVA,
        _NUM_FIELDS,
    };

    ImportThunk(const FileBytes& fbytes, const offset_t raw, std::string ordinal)
    : IHeader(fbytes, raw)
    , m_hintname()
    , m_ordinal(ordinal)
    {}

    ImportThunk(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    , m_hintname(fbytes, getStructPtr()->HintNameTableRVA - s_pDiskToMemDiff)
    {}

    const void* getFieldPtr(const int index) const override;
    const ArchType* getStructPtr() const { return static_cast<const ArchType*>(hdr()); }

    const ImportName* importname() const
    {
        return (getStructPtr()->OrdinalFlag) ? nullptr : &m_hintname;
    }

    const std::string* ordstr() const
    {
        return (getStructPtr()->OrdinalFlag) ? &m_ordinal : nullptr;
    }

    static const char* getFieldName(const int index);

private:
    ImportName m_hintname;
    std::string m_ordinal{};

    static size_t& s_pDiskToMemDiff;
};

/* Has members that point to a variable-length array of ImportThunks
 * and another variable-length array of ImportAddresses.
 * Length is equal to number of functions imported from the module.
 */
template <typename DescriptorType>
class GenericImportDescriptor final : public IHeader {
public:
    enum Fields : int {};

    GenericImportDescriptor(const PeFile& pe, const FileBytes& fbytes, const offset_t raw);

    GenericImportDescriptor(const GenericImportDescriptor& id)
    : IHeader(id)
    , m_thunks32(id.m_thunks32)
    , m_addresses32(id.m_addresses32)
    {}

    ~GenericImportDescriptor() {}

    GenericImportDescriptor& operator=(const GenericImportDescriptor& id)
    {
        if (this != &id) {
            IHeader::operator=(id);
            m_thunks32 = id.m_thunks32;
            m_addresses32 = id.m_addresses32;
        }
        return *this;
    }

    const void* getFieldPtr(const int index) const override;
    const DescriptorType* getStructPtr() const { return static_cast<const DescriptorType*>(hdr()); }

    const char* dllName() const;

    const std::vector<ImportThunk32>& getThunks32() const { return m_thunks32; }
    const std::vector<ImportThunk64>& getThunks64() const { return m_thunks64; }
    size_t thunksLength() const { return m_thunks32.size(); }

    const std::vector<ptr32_t>& getAddresses32() const { return m_addresses32; }
    const std::vector<ptr64_t>& getAddresses64() const { return m_addresses64; }
    size_t addressesLength() const { return m_addresses32.size(); }

    static const char* getFieldName(const int index);

private:
    union {
        std::vector<ImportThunk32> m_thunks32{};
        std::vector<ImportThunk64> m_thunks64;
    };

    union {
        std::vector<ptr32_t> m_addresses32{};
        std::vector<ptr64_t> m_addresses64;
    };

    static size_t& s_pDiskToMemDiff;

    template <int ILT, int IAT, int TIMESTAMP>
    void makeDescriptor(const PeFile& pe, const FileBytes& fbytes);

    template <typename ArchType>
    void readThunks(const FileBytes& fbytes, const offset_t raw);

    template <typename ArchType>
    void readAddresses(size_t raw);
};

template<>
enum GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::Fields : int {
    IMPORT_LOOKUP_TABLE_RVA,
    TIMESTAMP,
    FORWARDER_CHAIN,
    NAME_RVA,
    IMPORT_ADDRESS_TABLE_RVA,
    _NUM_FIELDS,
};

template<>
enum GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::Fields : int {
    ATTRIBUTES,
    NAME_RVA,
    MODULE_HANDLE_RVA,
    DELAY_IMPORT_ADDRESS_TABLE_RVA,
    DELAY_IMPORT_NAME_TABLE_RVA,
    BOUND_DELAY_IMPORT_TABLE_RVA,
    UNLOAD_DELAY_IMPORT_TABLE_RVA,
    TIMESTAMP,
    _NUM_FIELDS,
};

template class ImportThunk<IMAGE_THUNK_DATA32>;
template class ImportThunk<IMAGE_THUNK_DATA64>;

} // namespace Pepper

#endif
