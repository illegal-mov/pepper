#ifndef IAT_H
#define IAT_H

#include <sstream>
#include <vector>

#include "../generics/iDir.h"
#include "ImportsCommon.h"
#include "struct.h"

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;
template <typename T>
class AddressList;

/* TODO: `DataDir[IAT].rva()` is the same address as that of the first
 * ImportDescriptor.ImportAddressTableRVA, so structure the ImportDir
 * in such a way that I can point the IAT dir to part of the ImportDir
 */

typedef AddressList<int32_t> AddressList32;
typedef AddressList<int64_t> AddressList64;

/* A variable-length null-terminated array of import addresses
 */
template <typename T>
class AddressList final : public IHeader {
private:
    size_t m_length{};
    const T* addresses() const { return (T*)hdr(); }
public:
    AddressList()
    : IHeader()
    {}

    AddressList(const FileBytes &fbytes, const size_t raw, const size_t len);

    // member functions
    const void* getFieldPtr(const int index) const override;

    T address(const int index) const
    {
        T *ret = (T*)getFieldPtr(index);
        return (ret == nullptr) ? -1 : *ret;
    }

    size_t length() const { return m_length; }

    // static functions
    static const char* getFieldName(const int index);
};

/* Base of table of RVAs to function addresses
 * Points to same ImportLookups as ImportDir
 */
class IatDir final : public IDirectory {
private:
    union {
        std::vector<AddressList32> m_list32{};
        std::vector<AddressList64> m_list64;
    };

    template <typename T>
    void readAddrList(const FileBytes &fbytes, const DataDirectoryEntry &dde);
public:
    IatDir()
    : IDirectory()
    {}

    IatDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry& dde);

    IatDir(const IatDir &id)
    : IDirectory(id)
    , m_list32(id.m_list32)
    {}

    ~IatDir() {}

    const IatDir& operator=(const IatDir &id)
    {
        IDirectory::operator=(id);
        m_list32 = id.m_list32;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const std::vector<AddressList32>& list32() const { return m_list32; }
    const std::vector<AddressList64>& list64() const { return m_list64; }
    size_t length() const { return m_list32.size(); }

    // static functions
    static const char* getFieldName(const int index);
};

// variant declarations
template class AddressList<int32_t>;
template class AddressList<int64_t>;

} // namespace Pepper

#endif
