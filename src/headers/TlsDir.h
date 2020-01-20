#ifndef TLS_H
#define TLS_H

#include "../Conversion.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_TLS_DIRECTORY32 {
    int32_t RawDataStartVA;
    int32_t RawDataEndVA;
    int32_t AddressOfIndex;
    int32_t AddressOfCallbacks;
    int32_t SizeOfZeroFill;
    int32_t Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;

typedef struct _IMAGE_TLS_DIRECTORY64 {
    int64_t RawDataStartVA;
    int64_t RawDataEndVA;
    int64_t AddressOfIndex;
    int64_t AddressOfCallbacks;
    int32_t SizeOfZeroFill;
    int32_t Characteristics;
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
template <typename T>
class CallbacksTable final : public IHeader {
private:
    size_t m_length{};
    static size_t s_codeDiff; // AVAs in the Callbacks array point to .text

    const T* callbacks() const { return (T*)hdr(); }
public:
    CallbacksTable()
    : IHeader()
    {}

    CallbacksTable(const PeFile &pe, const FileBytes &fbytes, const size_t raw);

    // member functions
    const void* getFieldPtr(const int index) const override;

    // get array element or 0 if out of range
    T callbackAva(const int index) const
    {
        T *ret = (T*)getFieldPtr(index);
        return (ret == nullptr) ? 0 : *ret;
    }

    // get array element or 0 if out of range
    T callbackRaw(const int index) const
    {
        T *ret = (T*)getFieldPtr(index);
        return (ret == nullptr) ? 0 : *ret - s_codeDiff;
    }

    size_t length() const { return m_length; }

    // static functions
    static const char* getFieldName(const int index);
};

/* The TLS directory is a single 32-bit or 64-bit data structure.
 */
class TlsDir final : public IDirectory {
private:
    union {
        CallbacksTable<uint32_t> m_callbacks32;
        CallbacksTable<uint64_t> m_callbacks64;
    };
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

    TlsDir()
    : IDirectory()
    , m_callbacks32()
    {}

    TlsDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    ~TlsDir() {}

    // overloaded operators
    const TlsDir& operator=(const TlsDir &td)
    {
        IDirectory::operator=(td);
        m_callbacks32 = td.m_callbacks32;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_TLS_DIRECTORY32*   tls32() const { return (PIMAGE_TLS_DIRECTORY32)dir(); }
    const IMAGE_TLS_DIRECTORY64*   tls64() const { return (PIMAGE_TLS_DIRECTORY64)dir(); }
    const CallbacksTable<uint32_t>* cbt32() const { return Ident::dirExists(*this) ? &m_callbacks32 : nullptr; }
    const CallbacksTable<uint64_t>* cbt64() const { return Ident::dirExists(*this) ? &m_callbacks64 : nullptr; }

    // static functions
    static const char* getFieldName(const int index);
};

// variant declarations
template class CallbacksTable<uint32_t>;
template class CallbacksTable<uint64_t>;

} // namespace Pepper

#endif
