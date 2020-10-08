#include "../../include/headers/pe_structures/IatDir.h"

#include "../../include/headers/Identification.h"
#include "../../include/headers/Types.h"
#include "../../include/headers/pe_structures/DataDirectoryEntry.h"

using namespace Pepper;

template <typename ArchType>
AddressList<ArchType>::AddressList(const FileBytes& fbytes, const offset_t raw, const size_t addressListLength)
: IHeader(fbytes, raw)
, m_length(addressListLength)
{}

template <typename ArchType>
void IatDir::readAddrList(const FileBytes& fbytes, const DataDirectoryEntry& dde)
{
    const ArchType* addrs = static_cast<const ArchType*>(dir());
    size_t tableIndex = 0;
    size_t bytesRead = 0;
    while (bytesRead < dde.size()) {
        size_t addressListLength = 0;
        while (addrs[tableIndex] != 0) {
            addressListLength++;
            tableIndex++;
        }

        m_list32.emplace_back(fbytes, dirOffset() + bytesRead, addressListLength);

        bytesRead += (addressListLength+1) * sizeof(ArchType); // +1 because of null terminator
        tableIndex++; // `tableIndex` was left sitting on a null-terminator, so step over
    }
}

IatDir::IatDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        if (Ident::is32bit(pe)) {
            readAddrList<ptr32_t>(fbytes, dde);
        } else {
            readAddrList<ptr64_t>(fbytes, dde);
        }
    }
}

template <typename ArchType>
const char* AddressList<ArchType>::getFieldName(const int index)
{
    switch (index) {
        default: return "Import Address";
    }
}

template <typename ArchType>
const void* AddressList<ArchType>::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &getStructPtr()[uindex]
        : nullptr;
}

const char* IatDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* IatDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

