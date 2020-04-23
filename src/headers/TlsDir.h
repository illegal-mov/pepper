#ifndef TLS_H
#define TLS_H

#include "../Conversion.h"
#include "../Identification.h"
#include "../Types.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_TLS_DIRECTORY32 {
    int32_t  RawDataStartVA;
    int32_t  RawDataEndVA;
    uint32_t AddressOfIndex;
    uint32_t AddressOfCallbacks;
    uint32_t SizeOfZeroFill;
    int32_t  Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;

typedef struct _IMAGE_TLS_DIRECTORY64 {
    int64_t  RawDataStartVA;
    int64_t  RawDataEndVA;
    uint64_t AddressOfIndex;
    uint64_t AddressOfCallbacks;
    uint32_t SizeOfZeroFill;
    int32_t  Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* The CallbacksTable is a variable-length null-terimated array
 * of 32bit or 64bit AVAs to callback functions.
 */
template <typename ArchType>
class CallbacksTable final : public IHeader {
public:
    CallbacksTable() = default;

    CallbacksTable(const PeFile& pe, const FileBytes& fbytes, const offset_t raw);

    // member functions
    const void* getFieldPtr(const int index) const override;

    // get array element or 0 if out of range
    ArchType callbackAva(const int index) const
    {
        const ArchType *ret = static_cast<const ArchType*>(getFieldPtr(index));
        return (ret == nullptr) ? 0 : *ret;
    }

    // get array element or 0 if out of range
    ArchType callbackRaw(const int index) const
    {
        const ArchType *ret = static_cast<const ArchType*>(getFieldPtr(index));
        return (ret == nullptr) ? 0 : *ret - s_codeDiff;
    }

    size_t length() const { return m_length; }

    // static functions
    static const char* getFieldName(const int index);

private:
    size_t m_length{};
    static size_t s_codeDiff; // AVAs in the Callbacks array point to .text

    const ArchType* getStructPtr() const { return static_cast<const ArchType*>(hdr()); }
};

/* The TLS directory is a single 32-bit or 64-bit data structure.
 */
class TlsDir final : public IDirectory {
public:
    enum Fields {
        RAW_DATA_START_VA,
        RAW_DATA_END_VA,
        ADDRESS_OF_INDEX,
        ADDRESS_OF_CALLBACKS,
        SIZE_OF_ZERO_FILL,
        CHARACTERISTICS,
        _NUM_FIELDS,
    };

    TlsDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    ~TlsDir() {}

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_TLS_DIRECTORY32*   getStructPtr32() const { return static_cast<const IMAGE_TLS_DIRECTORY32*>(dir()); }
    const IMAGE_TLS_DIRECTORY64*   getStructPtr64() const { return static_cast<const IMAGE_TLS_DIRECTORY64*>(dir()); }
    const CallbacksTable<ptr32_t>& getCallbacks32() const { return m_callbacks32; }
    const CallbacksTable<ptr64_t>& getCallbacks64() const { return m_callbacks64; }

    // static functions
    static const char* getFieldName(const int index);

private:
    union {
        CallbacksTable<ptr32_t> m_callbacks32;
        CallbacksTable<ptr64_t> m_callbacks64;
    };
};

// variant declarations
template class CallbacksTable<ptr32_t>;
template class CallbacksTable<ptr64_t>;

} // namespace Pepper

#endif
