#ifndef IAT_H
#define IAT_H

#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"
#include "ImportsCommon.h"

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;
template <typename ArchType>
class AddressList;

/* TODO: `DataDir[IAT].rva()` is the same address as that of the first
 * ImportDescriptor.ImportAddressTableRVA, so structure the ImportDir
 * in such a way that I can point the IAT dir to part of the ImportDir
 */

using AddressList32 = AddressList<int32_t>;
using AddressList64 = AddressList<int64_t>;

/* A variable-length null-terminated array of import addresses
 */
template <typename ArchType>
class AddressList final : public IHeader {
public:
    AddressList(const FileBytes& fbytes, const offset_t raw, const size_t len);

    // member functions
    const void* getFieldPtr(const int index) const override;

    ArchType address(const int index) const
    {
        const ArchType *ret = static_cast<const ArchType*>(getFieldPtr(index));
        return (ret == nullptr) ? -1 : *ret;
    }

    size_t length() const { return m_length; }

    // static functions
    static const char* getFieldName(const int index);

private:
    size_t m_length{};
    const ArchType* getStructPtr() const { return static_cast<const ArchType*>(hdr()); }
};

/* Base of table of RVAs to function addresses
 * Points to same ImportLookups as ImportDir
 */
class IatDir final : public IDirectory {
public:
    IatDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    IatDir(const IatDir& id)
    : IDirectory(id)
    , m_list32(id.m_list32)
    {}

    ~IatDir() {}

    IatDir& operator=(const IatDir& id)
    {
        IDirectory::operator=(id);
        m_list32 = id.m_list32;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const std::vector<AddressList32>& getAddresses32() const { return m_list32; }
    const std::vector<AddressList64>& getAddresses64() const { return m_list64; }
    size_t length() const { return m_list32.size(); }

    // static functions
    static const char* getFieldName(const int index);

private:
    union {
        std::vector<AddressList32> m_list32{};
        std::vector<AddressList64> m_list64;
    };

    template <typename ArchType>
    void readAddrList(const FileBytes& fbytes, const DataDirectoryEntry& dde);
};

// variant declarations
template class AddressList<int32_t>;
template class AddressList<int64_t>;

} // namespace Pepper

#endif
