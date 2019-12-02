#ifndef IMPORTSCOMMON_H
#define IMPORTSCOMMON_H

#include <sstream>
#include <vector>

#include "../Identification.h"
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

// forward declarations
class FileBytes;
template <typename T>
class ImportThunk;

// typedefs that depend on the forward declaration
typedef ImportThunk<IMAGE_THUNK_DATA32> ImportThunk32;
typedef ImportThunk<IMAGE_THUNK_DATA64> ImportThunk64;

/* A 2-byte hint and imported function name as an ASCII string
 */
class ImportName final : public IHeader {
public:
    enum Fields {
        HINT,
        NAME,
        _NUM_FIELDS,
    };

    ImportName()
    : IHeader()
    {}

    ImportName(const FileBytes &fbytes, int raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const void* getFieldPtr(int index) const override;
    const IMAGE_IMPORT_BY_NAME* hintname() const
    {
        return (PIMAGE_IMPORT_BY_NAME)hdr();
    }

    // static functions
    static const char* getFieldName(int index);
};

/* An IMAGE_THUNK_DATA struct
 * RVA to ImportName as int32_t or int64_t
 */
template <typename T>
class ImportThunk final : public IHeader {
private:
    ImportName m_hintname;
    std::string m_ordstr{};

    static int *s_pDiskToMemDiff;
public:
    enum Fields {
        HINTNAMERVA,
        _NUM_FIELDS,
    };

    // constructor for import by ordinal (when OrdinalFlag is set)
    ImportThunk(const FileBytes &fbytes, int raw, std::string ordstr)
    : IHeader(fbytes, raw)
    , m_hintname()
    , m_ordstr(ordstr)
    {}

    // constructor for regular import
    ImportThunk(const FileBytes &fbytes, int raw)
    : IHeader(fbytes, raw)
    , m_hintname(fbytes, thunk()->HintNameTableRVA - *s_pDiskToMemDiff)
    {}

    // member functions
    const void* getFieldPtr(int index) const override;
    const T* thunk() const { return (T*)hdr(); }

    const ImportName* importname() const
    {
        return (thunk()->OrdinalFlag) ? nullptr : &m_hintname;
    }

    const std::string* ordstr() const
    {
        return (thunk()->OrdinalFlag) ? &m_ordstr : nullptr;
    }

    // static functions
    static const char* getFieldName(int index);
};

/* Has members that point to a variable-length array of ImportThunks
 * and another variable-length array of ImportAddresses.
 * length is equal to number of functions imported from the module.
 */
template <typename T>
class GenericImportDescriptor final : public IHeader {
private:
    // for data at ImportLookupTableRVA
    union {
        std::vector<ImportThunk32> m_thunks32{};
        std::vector<ImportThunk64> m_thunks64;
    };

    // for data at ImportAddressTableRVA
    union {
        std::vector<int32_t> m_addresses32{};
        std::vector<int64_t> m_addresses64;
    };

    static int *s_pDiskToMemDiff;

    template <int ILT, int IAT, int TIMESTAMP>
    void makeDescriptor(const PeFile &pe, const FileBytes &fbytes);

    template <typename U>
    void readThunks(const FileBytes &fbytes, int raw);

    template <typename U>
    void readAddresses(int raw);
public:
    // this enum is defined in template specializations
    enum Fields : int {};

    GenericImportDescriptor(const PeFile &pe, const FileBytes &fbytes, int raw);

    GenericImportDescriptor(const GenericImportDescriptor &id)
    : IHeader(id)
    , m_thunks32(id.m_thunks32)
    , m_addresses32(id.m_addresses32)
    {}

    ~GenericImportDescriptor() {}

    // overloaded operators
    const GenericImportDescriptor& operator=(const GenericImportDescriptor &id)
    {
        IHeader::operator=(id);
        m_thunks32 = id.m_thunks32;
        m_addresses32 = id.m_addresses32;
        return *this;
    }

    // member functions
    const void* getFieldPtr(int index) const override;
    const T* descriptor() const { return (T*)hdr(); }

    const char* dllName() const;

    const std::vector<ImportThunk32>& thunks32() const { return m_thunks32; }
    const std::vector<ImportThunk64>& thunks64() const { return m_thunks64; }
    int thunksLength() const { return (int)m_thunks32.size(); }

    const std::vector<int32_t>& addresses32() const { return m_addresses32; }
    const std::vector<int64_t>& addresses64() const { return m_addresses64; }
    int addressesLength() const { return (int)m_thunks32.size(); }

    // static functions
    static const char* getFieldName(int index);
};

// variant declarations
template <>
enum GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::Fields : int {
    IMPORT_LOOKUP_TABLE_RVA,
    TIMESTAMP,
    FORWARDER_CHAIN,
    NAME_RVA,
    IMPORT_ADDRESS_TABLE_RVA,
    _NUM_FIELDS,
};

template <>
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
} // namespace Pepper

#endif
